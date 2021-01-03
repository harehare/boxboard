use crate::domain::services::board_service::BoardService;
use std::sync::Arc;

#[derive(Clone)]
pub struct Context {
    pub board_service: Arc<BoardService>,
    pub uid: String,
}

impl juniper::Context for Context {}
