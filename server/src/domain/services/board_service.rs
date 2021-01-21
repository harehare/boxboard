use crate::domain::board::model::{
    Arrow, Board, BoardData, BoxData, Image, Markdown, Pen, Square, WebPage,
};
use crate::domain::board::repository::BoardRepository as BoardRepositoryTrait;
use crate::domain::values::board_id::BoardId;
use crate::domain::values::box_id::BoxId;
use crate::domain::values::user_id::UserId;
use crate::infra::board::repository::BoardRepository;
use anyhow::Result;

#[derive(Clone)]
pub struct BoardService {
    board_repository: BoardRepository,
}

impl BoardService {
    pub fn new(board_repository: BoardRepository) -> BoardService {
        BoardService { board_repository }
    }

    pub async fn get_board(&self, board_id: BoardId, user_id: UserId) -> Result<Board> {
        self.board_repository.find(board_id, user_id).await
    }

    pub async fn save_board(&self, input: BoardData) -> Result<()> {
        let board_id = self
            .board_repository
            .get_board_id(BoardId::new(input.board_id.clone()))
            .await;

        match board_id {
            Ok(id) => {
                self.board_repository
                    .update_board(BoardId::new(id), input)
                    .await
            }
            _ => self.board_repository.add_board(input).await,
        }
    }

    pub async fn delete_board(&self, board_id: BoardId) -> Result<()> {
        self.board_repository.delete_board(board_id).await
    }

    pub async fn add_box(
        &self,
        board_id: BoardId,
        user_id: UserId,
        input: BoxData,
    ) -> Result<BoxData> {
        let result = self
            .board_repository
            .add_box(board_id, user_id, input.clone())
            .await;

        result.map(|id| match input {
            BoxData::Markdown(markdown) => BoxData::Markdown(Markdown { id: id, ..markdown }),
            BoxData::WebPage(web) => BoxData::WebPage(WebPage { id: id, ..web }),
            BoxData::Image(image) => BoxData::Image(Image { id: id, ..image }),
            BoxData::Pen(pen) => BoxData::Pen(Pen { id: id, ..pen }),
            BoxData::Square(square) => BoxData::Square(Square { id: id, ..square }),
            BoxData::Arrow(arrow) => BoxData::Arrow(Arrow { id: id, ..arrow }),
        })
    }

    pub async fn update_box(
        &self,
        board_id: BoardId,
        box_id: BoxId,
        user_id: UserId,
        input: BoxData,
    ) -> Result<()> {
        self.board_repository
            .update_box(board_id, box_id, user_id, input)
            .await
    }

    pub async fn delete_box(&self, box_id: BoxId, user_id: UserId) -> Result<()> {
        self.board_repository.delete_box(box_id, user_id).await
    }
}
