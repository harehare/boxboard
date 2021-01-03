use juniper_warp;
use reqwest;
use reqwest::StatusCode;
use serde::Deserialize;
use std::net::SocketAddr;
use std::sync::Arc;
use warp::{http::Method, Filter};

mod auth;
mod domain;
mod errors;
mod graphql;
mod handler;
mod infra;
use crate::auth::Claims;
use crate::domain::services::board_service::BoardService;
use crate::graphql::context::Context;
use crate::infra::board::repository::BoardRepository;

#[derive(Deserialize, Debug, Clone)]
struct Config {
    pub server_host: String,
    pub server_port: String,
    pub faunadb_graphql_endpoint: String,
    pub faunadb_key: String,
    pub auth0_authority: String,
    pub auth0_audience: String,
}

fn crawl() -> warp::filters::BoxedFilter<()> {
    warp::path("crawl").boxed()
}

fn url() -> warp::filters::BoxedFilter<(handler::Url,)> {
    warp::path::param().boxed()
}

#[tokio::main]
async fn main() {
    pretty_env_logger::init();

    let config = match envy::from_env::<Config>() {
        Ok(val) => val,
        Err(err) => {
            println!("{}", err);
            std::process::exit(1);
        }
    };

    let cors = warp::cors()
        .allow_any_origin()
        .allow_credentials(true)
        .allow_headers(vec!["Content-Type", "Authorization"])
        .allow_methods(&[Method::POST, Method::GET]);

    let health_route = warp::get().and(warp::path("health")).map(|| StatusCode::OK);
    let crawl_route = warp::get().and(crawl()).and(url()).and_then(handler::crawl);

    let client = Arc::new(reqwest::Client::new());
    let temp_config = config.clone();
    let ctx = warp::post()
        .and(auth::with_auth(temp_config.auth0_authority.clone()))
        .map(move |claims: Claims| Context {
            board_service: Arc::new(BoardService::new(BoardRepository::new(
                temp_config.faunadb_key.clone(),
                temp_config.faunadb_graphql_endpoint.clone(),
                client.clone(),
            ))),
            uid: claims.sub,
        });
    // let ctx = warp::post().map(move || Context {
    //     board_service: Arc::new(BoardService::new(BoardRepository::new(
    //         temp_config.faunadb_key.clone(),
    //         temp_config.faunadb_graphql_endpoint.clone(),
    //         client.clone(),
    //     ))),
    //     uid: "".to_string(),
    // });

    let graphql_filter = juniper_warp::make_graphql_filter(graphql::schema::schema(), ctx.boxed());
    let graphql_route = warp::post()
        .and(warp::path("graphql"))
        .and(graphql_filter)
        .with(warp::compression::gzip());

    let playground_route = warp::get()
        .and(warp::path("playground"))
        .and(juniper_warp::playground_filter("/graphql", None));

    let routes = health_route
        .or(crawl_route)
        .or(graphql_route)
        .or(playground_route)
        .with(warp::log("boxboard"))
        .with(&cors);

    let server: SocketAddr = format!("{}:{}", config.server_host, config.server_port)
        .parse()
        .expect("Unable to parse address");
    warp::serve(routes.recover(handler::handle_rejection))
        .run(server)
        .await
}
