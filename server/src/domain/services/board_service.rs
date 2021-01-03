use crate::domain::board::model::{Arrow, Board, BoxData, Image, Markdown, Pen, Square, WebPage};
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
            BoxData::Markdown(_) => BoxData::Markdown(Markdown {
                id: id,
                x: 0,
                y: 0,
                width: 0,
                height: 0,
                order: 0,
                pinned: false,
                color: "".to_string(),
                text: "".to_string(),
                font_size: 0,
            }),
            BoxData::WebPage(_) => BoxData::WebPage(WebPage {
                id: id,
                x: 0,
                y: 0,
                width: 0,
                height: 0,
                order: 0,
                pinned: false,
                url: "".to_string(),
                image_url: None,
                title: None,
                description: None,
            }),
            BoxData::Image(_) => BoxData::Image(Image {
                id: id,
                x: 0,
                y: 0,
                width: 0,
                height: 0,
                order: 0,
                pinned: false,
                image: "".to_string(),
            }),
            BoxData::Pen(_) => BoxData::Pen(Pen {
                id: id,
                x: 0,
                y: 0,
                width: 0,
                height: 0,
                order: 0,
                pinned: false,
                draw_list: vec![],
            }),
            BoxData::Square(_) => BoxData::Square(Square {
                id: id,
                x: 0,
                y: 0,
                width: 0,
                height: 0,
                order: 0,
                pinned: false,
                color: "".to_string(),
            }),
            BoxData::Arrow(_) => BoxData::Arrow(Arrow {
                id: id,
                x: 0,
                y: 0,
                width: 0,
                height: 0,
                order: 0,
                pinned: false,
                color: "".to_string(),
                arrow_type: "".to_string(),
                angle: 0,
            }),
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
