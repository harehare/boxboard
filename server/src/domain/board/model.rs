use juniper;

#[derive(Clone, Debug, juniper::GraphQLObject)]
pub struct Markdown {
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

#[derive(Clone, Debug, juniper::GraphQLObject)]
pub struct WebPage {
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

#[derive(Clone, Debug, juniper::GraphQLObject)]
pub struct Image {
    pub id: String,
    pub x: i32,
    pub y: i32,
    pub width: i32,
    pub height: i32,
    pub order: i32,
    pub pinned: bool,
    pub image: String,
}

#[derive(Clone, Debug, juniper::GraphQLObject)]
pub struct Pen {
    pub id: String,
    pub x: i32,
    pub y: i32,
    pub width: i32,
    pub height: i32,
    pub order: i32,
    pub pinned: bool,
    pub draw_list: Vec<PenDraw>,
}

#[derive(Clone, Debug, juniper::GraphQLObject)]
pub struct PenDraw {
    pub color: String,
    pub draw: String,
}

#[derive(Clone, Debug, juniper::GraphQLObject)]
pub struct Square {
    pub id: String,
    pub x: i32,
    pub y: i32,
    pub width: i32,
    pub height: i32,
    pub order: i32,
    pub pinned: bool,
    pub color: String,
}

#[derive(Clone, Debug, juniper::GraphQLObject)]
pub struct Arrow {
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

#[derive(Clone, Debug, juniper::GraphQLUnion)]
pub enum BoxData {
    Markdown(Markdown),
    WebPage(WebPage),
    Image(Image),
    Pen(Pen),
    Square(Square),
    Arrow(Arrow),
}

#[derive(Clone, Debug, juniper::GraphQLObject)]
pub struct Board {
    pub id: String,
    pub boxes: Vec<BoxData>,
}

impl std::fmt::Display for BoxData {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            BoxData::Markdown(_) => write!(f, "markdown"),
            BoxData::WebPage(_) => write!(f, "web"),
            BoxData::Image(_) => write!(f, "image"),
            BoxData::Pen(_) => write!(f, "pen"),
            BoxData::Square(_) => write!(f, "square"),
            BoxData::Arrow(_) => write!(f, "arrow"),
        }
    }
}
