use thiserror::Error;

#[derive(Error, Debug)]
pub enum Error {
    #[error("fetch jwks error")]
    JwksFetchError,

    #[error("no authorization header found")]
    NoAuthHeaderFoundError,

    #[error("wrong authorization header format")]
    InvalidAuthHeaderFormatError,

    #[error("no user found for this token")]
    InvalidTokenError,

    #[error("error during authorization")]
    AuthorizationError,

    #[error("user is not unauthorized")]
    UnauthorizedError,

    #[error("unknown error")]
    Unknown,
}

impl warp::reject::Reject for Error {}

impl From<reqwest::Error> for Error {
    fn from(_error: reqwest::Error) -> Self {
        Error::JwksFetchError
    }
}
