use percent_encoding::percent_decode;
use reqwest;
use reqwest::StatusCode;
use scraper::{Html, Selector};
use serde::{Deserialize, Serialize};
use std::convert::Infallible;
use std::fmt;
use std::str::FromStr;

#[derive(Clone, Debug)]
pub struct Url(String);

impl fmt::Display for Url {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}

impl FromStr for Url {
    type Err = String;
    fn from_str(s: &str) -> Result<Self, Self::Err> {
        match percent_decode(s.as_bytes()).decode_utf8() {
            Ok(url) => {
                let size = s.chars().count();

                if !url.starts_with("http://") && !url.starts_with("https://") {
                    return Err("invalid url".to_string());
                }

                if size < 1 || size > 1000 {
                    return Err("invalid url".to_string());
                }
                Ok(Url(url.to_string()))
            }
            Err(_) => Err("invalid url".to_string()),
        }
    }
}

#[derive(Deserialize, Serialize, Clone)]
pub struct WebBox {
    url: Option<String>,
    title: Option<String>,
    description: Option<String>,
    image_url: Option<String>,
}

#[derive(Serialize)]
struct ErrorMessage {
    code: u16,
    message: String,
}

pub async fn crawl(Url(url): Url) -> Result<impl warp::Reply, warp::Rejection> {
    match reqwest::get(&url).await {
        Ok(res) => match res.text().await {
            Ok(html) => {
                let document = Html::parse_document(&html);
                let url_selector = Selector::parse("meta[property=\"og:url\"]").unwrap();
                let title_selector = Selector::parse("meta[property=\"og:title\"]").unwrap();
                let description_selector =
                    Selector::parse("meta[property=\"og:description\"]").unwrap();
                let image_selector = Selector::parse("meta[property=\"og:image\"]").unwrap();

                let web_box = WebBox {
                    url: document.select(&url_selector).next().and_then(|url| {
                        url.value()
                            .attr("content")
                            .and_then(|v| Some(v.to_string()))
                    }),
                    title: document.select(&title_selector).next().and_then(|title| {
                        title
                            .value()
                            .attr("content")
                            .and_then(|v| Some(v.to_string()))
                    }),
                    description: document.select(&description_selector).next().and_then(
                        |description| {
                            description
                                .value()
                                .attr("content")
                                .and_then(|v| Some(v.to_string()))
                        },
                    ),
                    image_url: document.select(&image_selector).next().and_then(|image| {
                        image
                            .value()
                            .attr("content")
                            .and_then(|v| Some(v.to_string()))
                    }),
                };
                Ok(warp::reply::json(&web_box))
            }
            Err(_) => Err(warp::reject::not_found()),
        },
        Err(_) => Err(warp::reject::not_found()),
    }
}

pub async fn handle_rejection(err: warp::Rejection) -> Result<impl warp::Reply, Infallible> {
    let code;
    let message;

    if err.is_not_found() {
        code = StatusCode::NOT_FOUND;
        message = "NOT_FOUND";
    } else if let Some(_) = err.find::<warp::reject::MethodNotAllowed>() {
        code = StatusCode::METHOD_NOT_ALLOWED;
        message = "METHOD_NOT_ALLOWED";
    } else {
        eprintln!("unhandled rejection: {:?}", err);
        code = StatusCode::INTERNAL_SERVER_ERROR;
        message = "UNHANDLED_REJECTION";
    }

    let json = warp::reply::json(&ErrorMessage {
        code: code.as_u16(),
        message: message.into(),
    });

    Ok(warp::reply::with_status(json, code))
}
