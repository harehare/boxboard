use crate::domain::board::model::{
    Arrow, Board, BoardData, BoxData, Image, Markdown, Pen, PenDraw, WebPage,
};
use crate::domain::board::repository;
use crate::domain::values::board_id::BoardId;
use crate::domain::values::box_id::BoxId;
use crate::domain::values::user_id::UserId;
use crate::errors::Error;
use anyhow::{anyhow, Result};
use async_trait::async_trait;
use graphql_client::*;
use std::convert::TryFrom;
use std::sync::Arc;

const DEFAULT_FONT_SIZE: i64 = 14;
const DEFAULT_STROKE_WIDTH: i64 = 2;

#[derive(GraphQLQuery)]
#[graphql(
    schema_path = "schema.graphql",
    query_path = "queries.graphql",
    response_derives = "Debug",
    normalization = "rust"
)]
struct FindBoard;

#[derive(GraphQLQuery)]
#[graphql(
    schema_path = "schema.graphql",
    query_path = "queries.graphql",
    response_derives = "Debug",
    normalization = "rust"
)]
struct FindBoardId;

#[derive(GraphQLQuery)]
#[graphql(
    schema_path = "schema.graphql",
    query_path = "queries.graphql",
    response_derives = "Debug",
    normalization = "rust"
)]
struct CreateBoard;

#[derive(GraphQLQuery)]
#[graphql(
    schema_path = "schema.graphql",
    query_path = "queries.graphql",
    response_derives = "Debug",
    normalization = "rust"
)]
struct UpdateBoard;

#[derive(GraphQLQuery)]
#[graphql(
    schema_path = "schema.graphql",
    query_path = "queries.graphql",
    response_derives = "Debug",
    normalization = "rust"
)]
struct DeleteBoard;

#[derive(GraphQLQuery)]
#[graphql(
    schema_path = "schema.graphql",
    query_path = "queries.graphql",
    response_derives = "Debug",
    normalization = "rust"
)]
struct CreateBox;

#[derive(GraphQLQuery)]
#[graphql(
    schema_path = "schema.graphql",
    query_path = "queries.graphql",
    response_derives = "Debug",
    normalization = "rust"
)]
struct UpdateBox;

#[derive(GraphQLQuery)]
#[graphql(
    schema_path = "schema.graphql",
    query_path = "queries.graphql",
    response_derives = "Debug",
    normalization = "rust"
)]
struct DeleteBox;

#[derive(Clone)]
pub struct BoardRepository {
    bearer_token: String,
    graphql_endpoint: String,
    client: Arc<reqwest::Client>,
}

impl BoardRepository {
    pub fn new(
        bearer_token: String,
        graphql_endpoint: String,
        client: Arc<reqwest::Client>,
    ) -> Self {
        Self {
            bearer_token,
            graphql_endpoint,
            client,
        }
    }
}

impl From<find_board::ResponseData> for Board {
    fn from(data: find_board::ResponseData) -> Self {
        match (data.find_board, data.find_boxes_in_board) {
            (Some(board), Some(item)) => Board {
                board: BoardData {
                    id: board.id,
                    board_id: board.board_id,
                    title: board.title.clone().unwrap_or("".to_string()),
                    x: TryFrom::try_from(board.x).unwrap(),
                    y: TryFrom::try_from(board.y).unwrap(),
                    scale: board.scale,
                },
                boxes: item
                    .data
                    .iter()
                    .map(|v| {
                        v.clone()
                            .as_ref()
                            .map(|b| match b.box_type.as_str() {
                                "markdown" => BoxData::Markdown(Markdown {
                                    id: b.id.to_owned(),
                                    x: TryFrom::try_from(b.x).unwrap(),
                                    y: TryFrom::try_from(b.y).unwrap(),
                                    width: TryFrom::try_from(b.width).unwrap(),
                                    height: TryFrom::try_from(b.height).unwrap(),
                                    order: TryFrom::try_from(b.order).unwrap(),
                                    pinned: b.pinned,
                                    color: b.color.clone().unwrap_or("rgba(0,0,0,0.0)".to_string()),
                                    text: b.text.clone().unwrap_or("".to_string()),
                                    font_size: TryFrom::try_from(
                                        b.font_size.unwrap_or(DEFAULT_FONT_SIZE),
                                    )
                                    .unwrap(),
                                }),
                                "image" => BoxData::Image(Image {
                                    id: b.id.to_owned(),
                                    x: TryFrom::try_from(b.x).unwrap(),
                                    y: TryFrom::try_from(b.y).unwrap(),
                                    width: TryFrom::try_from(b.width).unwrap(),
                                    height: TryFrom::try_from(b.height).unwrap(),
                                    order: TryFrom::try_from(b.order).unwrap(),
                                    pinned: b.pinned,
                                    image: b.url.clone().unwrap_or("".to_string()),
                                }),
                                "pen" => BoxData::Pen(Pen {
                                    id: b.id.to_owned(),
                                    x: TryFrom::try_from(b.x).unwrap(),
                                    y: TryFrom::try_from(b.y).unwrap(),
                                    width: TryFrom::try_from(b.width).unwrap(),
                                    height: TryFrom::try_from(b.height).unwrap(),
                                    order: TryFrom::try_from(b.order).unwrap(),
                                    pinned: b.pinned,
                                    draw_list: match (b.pen_draws.clone(), b.pen_colors.clone()) {
                                        (Some(draws), Some(colors)) => draws
                                            .iter()
                                            .zip(colors.iter())
                                            .map(|d| match d {
                                                (draw, color) => PenDraw {
                                                    draw: draw.to_string(),
                                                    color: color.to_string(),
                                                },
                                            })
                                            .collect(),
                                        _ => vec![],
                                    },
                                }),
                                "web" => BoxData::WebPage(WebPage {
                                    id: b.id.to_owned(),
                                    x: TryFrom::try_from(b.x).unwrap(),
                                    y: TryFrom::try_from(b.y).unwrap(),
                                    width: TryFrom::try_from(b.width).unwrap(),
                                    height: TryFrom::try_from(b.height).unwrap(),
                                    order: TryFrom::try_from(b.order).unwrap(),
                                    pinned: b.pinned,
                                    url: b.url.clone().unwrap_or("".to_string()),
                                    image_url: b.image_url.clone(),
                                    title: b.title.clone(),
                                    description: b.description.clone(),
                                }),
                                "arrow" => BoxData::Arrow(Arrow {
                                    id: b.id.to_owned(),
                                    x: TryFrom::try_from(b.x).unwrap(),
                                    y: TryFrom::try_from(b.y).unwrap(),
                                    width: TryFrom::try_from(b.width).unwrap(),
                                    height: TryFrom::try_from(b.height).unwrap(),
                                    order: TryFrom::try_from(b.order).unwrap(),
                                    pinned: b.pinned,
                                    color: b.color.clone().unwrap_or("rgba(0,0,0,0.0)".to_string()),
                                    arrow_type: b.arrow_type.clone().unwrap_or("arrow".to_string()),
                                    angle: TryFrom::try_from(b.angle).unwrap(),
                                    stroke_width: TryFrom::try_from(
                                        b.stroke_width.unwrap_or(DEFAULT_STROKE_WIDTH),
                                    )
                                    .unwrap(),
                                }),
                                _ => BoxData::Markdown(Markdown {
                                    id: "".to_owned(),
                                    x: 0,
                                    y: 0,
                                    width: 0,
                                    height: 0,
                                    order: 0,
                                    pinned: false,
                                    color: "rgba(0,0,0,0.0)".to_string(),
                                    text: "".to_string(),
                                    font_size: TryFrom::try_from(DEFAULT_FONT_SIZE).unwrap(),
                                }),
                            })
                            // TODO: error
                            .unwrap_or(BoxData::Markdown(Markdown {
                                id: "".to_owned(),
                                x: 0,
                                y: 0,
                                width: 0,
                                height: 0,
                                order: 0,
                                pinned: false,
                                color: "rgba(0,0,0,0.0)".to_string(),
                                text: "".to_string(),
                                font_size: TryFrom::try_from(DEFAULT_FONT_SIZE).unwrap(),
                            }))
                    })
                    .collect(),
            },
            _ => Board {
                board: BoardData {
                    id: "".to_string(),
                    board_id: "".to_string(),
                    title: "".to_string(),
                    x: 0,
                    y: 0,
                    scale: 1.0,
                },
                boxes: vec![],
            },
        }
    }
}

#[async_trait]
impl repository::BoardRepository for BoardRepository {
    async fn get_board_id(&self, id: BoardId) -> Result<String> {
        use find_board_id::*;

        let q = FindBoardId::build_query(Variables { id: id.to_string() });
        let res: Response<ResponseData> = self
            .client
            .post(&self.graphql_endpoint)
            .header("Authorization", format!("Bearer {}", self.bearer_token))
            .json(&q)
            .send()
            .await?
            .json()
            .await?;
        match (res.data, res.errors) {
            (Some(data), _) => match data.find_board {
                Some(v) => Ok(v.id),
                None => Err(anyhow!(Error::NotFound)),
            },
            (None, Some(errors)) => Err(anyhow!(errors
                .iter()
                .map(|e| e.message.to_string())
                .collect::<Vec<_>>()
                .join(","))),
            _ => Err(anyhow!(Error::Unknown)),
        }
    }

    async fn find(&self, id: BoardId, user_id: UserId) -> Result<Board> {
        let q = FindBoard::build_query(find_board::Variables {
            id: id.to_string(),
            user_id: user_id.to_string(),
        });
        let res: Response<find_board::ResponseData> = self
            .client
            .post(&self.graphql_endpoint)
            .header("Authorization", format!("Bearer {}", self.bearer_token))
            .json(&q)
            .send()
            .await?
            .json()
            .await?;
        match (res.data, res.errors) {
            (Some(data), _) => Ok(Board::from(data)),
            (None, Some(errors)) => Err(anyhow!(errors
                .iter()
                .map(|e| e.message.to_string())
                .collect::<Vec<_>>()
                .join(","))),
            _ => Err(anyhow!(Error::Unknown)),
        }
    }

    async fn add_board(&self, board: BoardData) -> Result<()> {
        use create_board::*;
        let body = CreateBoard::build_query(Variables {
            data: BoardInput {
                board_id: board.board_id,
                title: Some(board.title),
                x: TryFrom::try_from(board.x).unwrap(),
                y: TryFrom::try_from(board.y).unwrap(),
                scale: board.scale,
            },
        });

        let res: Response<ResponseData> = self
            .client
            .post(&self.graphql_endpoint)
            .header("Authorization", format!("Bearer {}", self.bearer_token))
            .json(&body)
            .send()
            .await?
            .json()
            .await?;

        match (res.data, res.errors) {
            (Some(_), None) => Ok(()),
            (None, Some(errors)) => Err(anyhow!(errors
                .iter()
                .map(|e| e.message.to_string())
                .collect::<Vec<_>>()
                .join(","))),
            _ => Err(anyhow!(Error::Unknown)),
        }
    }

    async fn update_board(&self, board_id: BoardId, board: BoardData) -> Result<()> {
        use update_board::*;
        let body = UpdateBoard::build_query(Variables {
            id: board_id.to_string(),
            data: BoardInput {
                board_id: board.board_id,
                title: Some(board.title),
                x: TryFrom::try_from(board.x).unwrap(),
                y: TryFrom::try_from(board.y).unwrap(),
                scale: board.scale,
            },
        });

        let res: Response<ResponseData> = self
            .client
            .post(&self.graphql_endpoint)
            .header("Authorization", format!("Bearer {}", self.bearer_token))
            .json(&body)
            .send()
            .await?
            .json()
            .await?;

        match (res.data, res.errors) {
            (Some(_num), _) => Ok(()),
            (None, Some(errors)) => Err(anyhow!(errors
                .iter()
                .map(|e| e.message.to_string())
                .collect::<Vec<_>>()
                .join(","))),
            _ => Err(anyhow!(Error::Unknown)),
        }
    }

    async fn delete_board(&self, board_id: BoardId) -> Result<()> {
        use delete_board::*;
        let body = DeleteBoard::build_query(Variables {
            id: board_id.to_string(),
        });

        // TODO: check user
        let res: Response<ResponseData> = self
            .client
            .post(&self.graphql_endpoint)
            .header("Authorization", format!("Bearer {}", self.bearer_token))
            .json(&body)
            .send()
            .await?
            .json()
            .await?;
        match (res.data, res.errors) {
            (Some(_num), None) => Ok(()),
            (None, Some(errors)) => Err(anyhow!(errors
                .iter()
                .map(|e| e.message.to_string())
                .collect::<Vec<_>>()
                .join(","))),
            _ => Err(anyhow!(Error::Unknown)),
        }
    }

    async fn add_box(
        &self,
        board_id: BoardId,
        user_id: UserId,
        box_data: BoxData,
    ) -> Result<String> {
        use create_box::*;
        let body = CreateBox::build_query(Variables {
            data: match box_data.clone() {
                BoxData::Markdown(markdown) => BoxInput {
                    board_id: board_id.to_string(),
                    user_id: user_id.to_string(),
                    x: TryFrom::try_from(markdown.x).unwrap(),
                    y: TryFrom::try_from(markdown.y).unwrap(),
                    width: TryFrom::try_from(markdown.width).unwrap(),
                    height: TryFrom::try_from(markdown.height).unwrap(),
                    order: TryFrom::try_from(markdown.order).unwrap(),
                    box_type: "markdown".to_string(),
                    pinned: markdown.pinned,
                    text: Some(markdown.text),
                    font_size: Some(TryFrom::try_from(markdown.font_size).unwrap()),
                    url: None,
                    image_url: None,
                    title: None,
                    description: None,
                    pen_colors: None,
                    pen_draws: None,
                    color: Some(markdown.color),
                    arrow_type: None,
                    angle: 0,
                    stroke_width: None,
                },
                BoxData::WebPage(web_page) => BoxInput {
                    board_id: board_id.to_string(),
                    user_id: user_id.to_string(),
                    x: TryFrom::try_from(web_page.x).unwrap(),
                    y: TryFrom::try_from(web_page.y).unwrap(),
                    width: TryFrom::try_from(web_page.width).unwrap(),
                    height: TryFrom::try_from(web_page.height).unwrap(),
                    order: TryFrom::try_from(web_page.order).unwrap(),
                    box_type: "web".to_string(),
                    pinned: web_page.pinned,
                    text: None,
                    font_size: None,
                    url: Some(web_page.url),
                    image_url: web_page.image_url,
                    title: web_page.title,
                    description: web_page.description,
                    pen_colors: None,
                    pen_draws: None,
                    color: None,
                    arrow_type: None,
                    angle: 0,
                    stroke_width: None,
                },
                BoxData::Image(image) => BoxInput {
                    board_id: board_id.to_string(),
                    user_id: user_id.to_string(),
                    x: TryFrom::try_from(image.x).unwrap(),
                    y: TryFrom::try_from(image.y).unwrap(),
                    width: TryFrom::try_from(image.width).unwrap(),
                    height: TryFrom::try_from(image.height).unwrap(),
                    order: TryFrom::try_from(image.order).unwrap(),
                    pinned: image.pinned,
                    box_type: "image".to_string(),
                    url: Some(image.image),
                    text: None,
                    font_size: None,
                    image_url: None,
                    title: None,
                    description: None,
                    pen_colors: None,
                    pen_draws: None,
                    color: None,
                    arrow_type: None,
                    angle: 0,
                    stroke_width: None,
                },
                BoxData::Pen(pen) => BoxInput {
                    board_id: board_id.to_string(),
                    user_id: user_id.to_string(),
                    x: TryFrom::try_from(pen.x).unwrap(),
                    y: TryFrom::try_from(pen.y).unwrap(),
                    width: TryFrom::try_from(pen.width).unwrap(),
                    height: TryFrom::try_from(pen.height).unwrap(),
                    order: TryFrom::try_from(pen.order).unwrap(),
                    box_type: "pen".to_string(),
                    pinned: pen.pinned,
                    url: None,
                    text: None,
                    font_size: None,
                    image_url: None,
                    title: None,
                    description: None,
                    pen_colors: Some(pen.draw_list.iter().map(|d| d.color.to_string()).collect()),
                    pen_draws: Some(pen.draw_list.iter().map(|d| d.draw.to_string()).collect()),
                    color: None,
                    arrow_type: None,
                    angle: 0,
                    stroke_width: None,
                },
                BoxData::Arrow(arrow) => BoxInput {
                    board_id: board_id.to_string(),
                    user_id: user_id.to_string(),
                    x: TryFrom::try_from(arrow.x).unwrap(),
                    y: TryFrom::try_from(arrow.y).unwrap(),
                    width: TryFrom::try_from(arrow.width).unwrap(),
                    height: TryFrom::try_from(arrow.height).unwrap(),
                    order: TryFrom::try_from(arrow.order).unwrap(),
                    box_type: "arrow".to_string(),
                    pinned: arrow.pinned,
                    url: None,
                    text: None,
                    font_size: None,
                    image_url: None,
                    title: None,
                    description: None,
                    pen_colors: None,
                    pen_draws: None,
                    color: Some(arrow.color),
                    arrow_type: Some(arrow.arrow_type),
                    angle: 0,
                    stroke_width: Some(TryFrom::try_from(arrow.stroke_width).unwrap()),
                },
            },
        });

        let res: Response<ResponseData> = self
            .client
            .post(&self.graphql_endpoint)
            .header("Authorization", format!("Bearer {}", self.bearer_token))
            .json(&body)
            .send()
            .await?
            .json()
            .await?;

        match (res.data, res.errors) {
            (Some(data), None) => Ok(data.create_box.id),
            (None, Some(errors)) => Err(anyhow!(errors
                .iter()
                .map(|e| e.message.to_string())
                .collect::<Vec<_>>()
                .join(","))),
            _ => Err(anyhow!(Error::Unknown)),
        }
    }

    async fn update_box(
        &self,
        board_id: BoardId,
        box_id: BoxId,
        user_id: UserId,
        box_data: BoxData,
    ) -> Result<()> {
        use update_box::*;
        let body = UpdateBox::build_query(Variables {
            id: box_id.to_string(),
            data: match box_data.clone() {
                BoxData::Markdown(markdown) => BoxInput {
                    board_id: board_id.to_string(),
                    user_id: user_id.to_string(),
                    x: TryFrom::try_from(markdown.x).unwrap(),
                    y: TryFrom::try_from(markdown.y).unwrap(),
                    width: TryFrom::try_from(markdown.width).unwrap(),
                    height: TryFrom::try_from(markdown.height).unwrap(),
                    order: TryFrom::try_from(markdown.order).unwrap(),
                    box_type: "markdown".to_string(),
                    pinned: markdown.pinned,
                    url: None,
                    text: Some(markdown.text),
                    font_size: Some(TryFrom::try_from(markdown.font_size).unwrap()),
                    image_url: None,
                    title: None,
                    description: None,
                    pen_colors: None,
                    pen_draws: None,
                    color: Some(markdown.color),
                    arrow_type: None,
                    angle: 0,
                    stroke_width: None,
                },
                BoxData::WebPage(web_page) => BoxInput {
                    board_id: board_id.to_string(),
                    user_id: user_id.to_string(),
                    x: TryFrom::try_from(web_page.x).unwrap(),
                    y: TryFrom::try_from(web_page.y).unwrap(),
                    width: TryFrom::try_from(web_page.width).unwrap(),
                    height: TryFrom::try_from(web_page.height).unwrap(),
                    order: TryFrom::try_from(web_page.order).unwrap(),
                    box_type: "web".to_string(),
                    pinned: web_page.pinned,
                    url: Some(web_page.url),
                    text: None,
                    font_size: None,
                    image_url: web_page.image_url,
                    title: web_page.title,
                    description: web_page.description,
                    pen_colors: None,
                    pen_draws: None,
                    color: None,
                    arrow_type: None,
                    angle: 0,
                    stroke_width: None,
                },
                BoxData::Image(image) => BoxInput {
                    board_id: board_id.to_string(),
                    user_id: user_id.to_string(),
                    x: TryFrom::try_from(image.x).unwrap(),
                    y: TryFrom::try_from(image.y).unwrap(),
                    width: TryFrom::try_from(image.width).unwrap(),
                    height: TryFrom::try_from(image.height).unwrap(),
                    order: TryFrom::try_from(image.order).unwrap(),
                    box_type: "image".to_string(),
                    pinned: image.pinned,
                    url: Some(image.image),
                    text: None,
                    font_size: None,
                    image_url: None,
                    title: None,
                    description: None,
                    pen_colors: None,
                    pen_draws: None,
                    color: None,
                    arrow_type: None,
                    angle: 0,
                    stroke_width: None,
                },
                BoxData::Pen(pen) => BoxInput {
                    board_id: board_id.to_string(),
                    user_id: user_id.to_string(),
                    x: TryFrom::try_from(pen.x).unwrap(),
                    y: TryFrom::try_from(pen.y).unwrap(),
                    width: TryFrom::try_from(pen.width).unwrap(),
                    height: TryFrom::try_from(pen.height).unwrap(),
                    order: TryFrom::try_from(pen.order).unwrap(),
                    box_type: "pen".to_string(),
                    pinned: pen.pinned,
                    url: None,
                    text: None,
                    font_size: None,
                    image_url: None,
                    title: None,
                    description: None,
                    pen_colors: Some(pen.draw_list.iter().map(|d| d.color.to_string()).collect()),
                    pen_draws: Some(pen.draw_list.iter().map(|d| d.draw.to_string()).collect()),
                    color: None,
                    arrow_type: None,
                    angle: 0,
                    stroke_width: None,
                },
                BoxData::Arrow(arrow) => BoxInput {
                    board_id: board_id.to_string(),
                    user_id: user_id.to_string(),
                    x: TryFrom::try_from(arrow.x).unwrap(),
                    y: TryFrom::try_from(arrow.y).unwrap(),
                    width: TryFrom::try_from(arrow.width).unwrap(),
                    height: TryFrom::try_from(arrow.height).unwrap(),
                    order: TryFrom::try_from(arrow.order).unwrap(),
                    box_type: "arrow".to_string(),
                    pinned: arrow.pinned,
                    url: None,
                    text: None,
                    font_size: None,
                    image_url: None,
                    title: None,
                    description: None,
                    pen_colors: None,
                    pen_draws: None,
                    color: Some(arrow.color),
                    arrow_type: Some(arrow.arrow_type),
                    angle: TryFrom::try_from(arrow.angle).unwrap(),
                    stroke_width: Some(TryFrom::try_from(arrow.stroke_width).unwrap()),
                },
            },
        });

        let res: Response<ResponseData> = self
            .client
            .post(&self.graphql_endpoint)
            .header("Authorization", format!("Bearer {}", self.bearer_token))
            .json(&body)
            .send()
            .await?
            .json()
            .await?;
        match (res.data, res.errors) {
            (Some(_num), _) => Ok(()),
            (None, Some(errors)) => Err(anyhow!(errors
                .iter()
                .map(|e| e.message.to_string())
                .collect::<Vec<_>>()
                .join(","))),
            _ => Err(anyhow!(Error::Unknown)),
        }
    }

    async fn delete_box(&self, box_id: BoxId, _user_id: UserId) -> Result<()> {
        use delete_box::*;
        let body = DeleteBox::build_query(Variables {
            id: box_id.to_string(),
        });

        // TODO: check user
        let res: Response<ResponseData> = self
            .client
            .post(&self.graphql_endpoint)
            .header("Authorization", format!("Bearer {}", self.bearer_token))
            .json(&body)
            .send()
            .await?
            .json()
            .await?;
        match (res.data, res.errors) {
            (Some(_num), None) => Ok(()),
            (None, Some(errors)) => Err(anyhow!(errors
                .iter()
                .map(|e| e.message.to_string())
                .collect::<Vec<_>>()
                .join(","))),
            _ => Err(anyhow!(Error::Unknown)),
        }
    }
}
