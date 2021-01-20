use crate::domain::board::model::{Arrow, BoxData, Image, Markdown, Pen, PenDraw, Square, WebPage};
use juniper;

#[derive(juniper::GraphQLInputObject)]
pub struct MarkdownInput {
    pub id: String,
    pub x: i32,
    pub y: i32,
    pub width: i32,
    pub height: i32,
    pub order: i32,
    pub pinned: bool,
    pub color: String,
    pub text: String,
    pub font_size: i32,
}

#[derive(juniper::GraphQLInputObject)]
pub struct WebPageInput {
    pub id: String,
    pub x: i32,
    pub y: i32,
    pub width: i32,
    pub height: i32,
    pub order: i32,
    pub pinned: bool,
    pub url: String,
    pub image_url: Option<String>,
    pub title: Option<String>,
    pub description: Option<String>,
}

#[derive(juniper::GraphQLInputObject)]
pub struct ImageInput {
    pub id: String,
    pub x: i32,
    pub y: i32,
    pub width: i32,
    pub height: i32,
    pub order: i32,
    pub pinned: bool,
    pub image: String,
}

#[derive(juniper::GraphQLInputObject)]
pub struct PenInput {
    pub id: String,
    pub x: i32,
    pub y: i32,
    pub width: i32,
    pub height: i32,
    pub order: i32,
    pub pinned: bool,
    pub draw_list: Vec<PenDrawInput>,
}

#[derive(juniper::GraphQLInputObject)]
pub struct PenDrawInput {
    draw: String,
    color: String,
}

#[derive(juniper::GraphQLInputObject)]
pub struct SquareInput {
    pub id: String,
    pub x: i32,
    pub y: i32,
    pub width: i32,
    pub height: i32,
    pub order: i32,
    pub pinned: bool,
    pub color: String,
}

#[derive(juniper::GraphQLInputObject)]
pub struct ArrowInput {
    pub id: String,
    pub x: i32,
    pub y: i32,
    pub width: i32,
    pub height: i32,
    pub order: i32,
    pub pinned: bool,
    pub color: String,
    pub arrow_type: String,
    pub angle: i32,
    pub stroke_width: i32,
}

#[derive(juniper::GraphQLInputObject)]
pub struct BoardInput {
    pub board_id: String,
    pub title: String,
    pub x: i32,
    pub y: i32,
    pub scale: f64,
}

impl From<MarkdownInput> for BoxData {
    fn from(data: MarkdownInput) -> Self {
        BoxData::Markdown(Markdown {
            id: data.id,
            x: data.x,
            y: data.y,
            width: data.width,
            height: data.height,
            order: data.order,
            pinned: data.pinned,
            color: data.color,
            text: data.text,
            font_size: data.font_size,
        })
    }
}

impl From<WebPageInput> for BoxData {
    fn from(data: WebPageInput) -> Self {
        BoxData::WebPage(WebPage {
            id: data.id,
            x: data.x,
            y: data.y,
            width: data.width,
            height: data.height,
            order: data.order,
            pinned: data.pinned,
            url: data.url,
            image_url: data.image_url,
            title: data.title,
            description: data.description,
        })
    }
}

impl From<ImageInput> for BoxData {
    fn from(data: ImageInput) -> Self {
        BoxData::Image(Image {
            id: data.id,
            x: data.x,
            y: data.y,
            width: data.width,
            height: data.height,
            order: data.order,
            pinned: data.pinned,
            image: data.image,
        })
    }
}

impl From<PenInput> for BoxData {
    fn from(data: PenInput) -> Self {
        BoxData::Pen(Pen {
            id: data.id,
            x: data.x,
            y: data.y,
            width: data.width,
            height: data.height,
            order: data.order,
            pinned: data.pinned,
            draw_list: data
                .draw_list
                .iter()
                .map(|d| PenDraw {
                    color: d.color.clone(),
                    draw: d.draw.clone(),
                })
                .collect(),
        })
    }
}

impl From<SquareInput> for BoxData {
    fn from(data: SquareInput) -> Self {
        BoxData::Square(Square {
            id: data.id,
            x: data.x,
            y: data.y,
            width: data.width,
            height: data.height,
            order: data.order,
            pinned: data.pinned,
            color: data.color,
        })
    }
}

impl From<ArrowInput> for BoxData {
    fn from(data: ArrowInput) -> Self {
        BoxData::Arrow(Arrow {
            id: data.id,
            x: data.x,
            y: data.y,
            width: data.width,
            height: data.height,
            order: data.order,
            pinned: data.pinned,
            color: data.color,
            arrow_type: data.arrow_type,
            angle: data.angle,
            stroke_width: data.stroke_width,
        })
    }
}
