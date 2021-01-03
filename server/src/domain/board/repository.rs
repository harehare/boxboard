use crate::domain::board::model::{Board, BoxData};
use crate::domain::values::board_id::BoardId;
use crate::domain::values::box_id::BoxId;
use crate::domain::values::user_id::UserId;

use anyhow::Result;
use async_trait::async_trait;

#[async_trait]
pub trait BoardRepository {
    async fn find(&self, board_id: BoardId, user_id: UserId) -> Result<Board>;
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
}
