use crate::domain::board::model::{Board, BoardData, BoxData};
use crate::domain::values::board_id::BoardId;
use crate::domain::values::box_id::BoxId;
use crate::domain::values::user_id::UserId;

use anyhow::Result;
use async_trait::async_trait;

#[async_trait]
pub trait BoardRepository {
    async fn find(&self, board_id: BoardId, user_id: UserId) -> Result<Board>;
    async fn get_board_id(&self, board_id: BoardId) -> Result<String>;
    async fn add_box(
        &self,
        board_id: BoardId,
        user_id: UserId,
        box_data: BoxData,
    ) -> Result<String>;
    async fn update_box(
        &self,
        board_id: BoardId,
        box_id: BoxId,
        user_id: UserId,
        box_data: BoxData,
    ) -> Result<()>;
    async fn delete_box(&self, board_id: BoxId, user_id: UserId) -> Result<()>;
    async fn add_board(&self, board: BoardData) -> Result<()>;
    async fn update_board(&self, board_id: BoardId, board: BoardData) -> Result<()>;
    async fn delete_board(&self, board_id: BoardId) -> Result<()>;
}
