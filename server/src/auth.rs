use crate::errors::Error;
use alcoholic_jwt::{token_kid, validate, Validation, JWKS};
use reqwest;
use serde::{Deserialize, Serialize};
use warp::{
    filters::header::headers_cloned,
    http::header::{HeaderMap, HeaderValue, AUTHORIZATION},
    Filter,
};

const BEARER: &str = "Bearer ";

#[derive(Debug, Deserialize, Serialize)]
pub struct Claims {
    pub sub: String,
    pub exp: usize,
}

async fn fetch_jwks(uri: &str) -> Result<JWKS, Error> {
    reqwest::get(uri)
        .await?
        .json::<JWKS>()
        .await
        .map_err(|_| Error::JwksFetchError)
}

pub fn with_auth(
    authority: String,
) -> impl warp::Filter<Extract = (Claims,), Error = warp::Rejection> + Clone {
    headers_cloned()
        .map(move |headers: HeaderMap<HeaderValue>| (headers, authority.clone()))
        .and_then(authorize)
}

async fn validate_token(token: &str, authority: String) -> Result<Claims, Error> {
    let jwks = fetch_jwks(&format!("{}{}", authority, ".well-known/jwks.json")).await?;
    let kid = match token_kid(token) {
        Ok(res) => res.expect("failed to decode kid"),
        Err(_) => return Err(Error::UnauthorizedError),
    };
    let jwk = jwks.find(&kid).expect("Specified key not found in set");
    let res = validate(
        token,
        jwk,
        vec![
            Validation::Issuer(authority.to_string()),
            Validation::SubjectPresent,
        ],
    );

    res.map_err(|_| Error::InvalidTokenError)
        .and_then(|r| serde_json::from_value(r.claims).map_err(|_| Error::AuthorizationError))
}

async fn authorize(
    (headers, authority): (HeaderMap<HeaderValue>, String),
) -> Result<Claims, warp::Rejection> {
    match get_jwt_from_header(&headers) {
        Ok(jwt) => validate_token(jwt.as_str(), authority)
            .await
            .map_err(|e| warp::reject::custom(e)),

        Err(e) => Err(warp::reject::custom(e)),
    }
}

fn get_jwt_from_header(headers: &HeaderMap<HeaderValue>) -> Result<String, Error> {
    let header = match headers.get(AUTHORIZATION) {
        Some(v) => v,
        None => return Err(Error::NoAuthHeaderFoundError),
    };
    let auth_header = match std::str::from_utf8(header.as_bytes()) {
        Ok(v) => v,
        Err(_) => return Err(Error::InvalidAuthHeaderFormatError),
    };
    if !auth_header.starts_with(BEARER) {
        return Err(Error::InvalidTokenError);
    }
    Ok(auth_header.trim_start_matches(BEARER).to_owned())
}
