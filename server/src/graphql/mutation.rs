use crate::domain::board::model::{BoardData, BoxData};
use crate::domain::values::board_id::BoardId;
use crate::domain::values::box_id::BoxId;
use crate::domain::values::user_id::UserId;
use crate::graphql::context::Context;
use crate::graphql::input::{
    ArrowInput, BoardInput, ImageInput, MarkdownInput, PenInput, SquareInput, WebPageInput,
};
use juniper;

pub struct MutationRoot;

async fn add_box(
    ctx: &Context,
    board_id: juniper::ID,
    box_data: BoxData,
) -> juniper::FieldResult<BoxData> {
    let res = ctx
        .board_service
        .add_box(
            BoardId::new(board_id.to_string()),
            UserId::new(ctx.uid.clone()),
            box_data.clone(),
        )
        .await;
    res.map_err(|e| juniper::FieldError::from(e))
}

async fn update_box(
    ctx: &Context,
    board_id: juniper::ID,
    box_id: juniper::ID,
    box_data: BoxData,
) -> juniper::FieldResult<BoxData> {
    let res = ctx
        .board_service
        .update_box(
            BoardId::new(board_id.to_string()),
            BoxId::new(box_id.to_string()),
            UserId::new(ctx.uid.clone()),
            box_data.clone(),
        )
        .await;
    res.and_then(|_| Ok(box_data))
        .map_err(|e| juniper::FieldError::from(e))
}

async fn delete_box(
    ctx: &Context,
    box_id: juniper::ID,
    box_data: BoxData,
) -> juniper::FieldResult<BoxData> {
    let res = ctx
        .board_service
        .delete_box(BoxId::new(box_id.to_string()), UserId::new(ctx.uid.clone()))
        .await;
    res.and_then(|_| Ok(box_data))
        .map_err(|e| juniper::FieldError::from(e))
}

#[juniper::graphql_object(Context = Context)]
impl MutationRoot {
    async fn save_board(
        ctx: &Context,
        board_id: juniper::ID,
        input: BoardInput,
    ) -> juniper::FieldResult<BoardData> {
        let board_data = BoardData {
            id: board_id.to_string(),
            board_id: board_id.to_string(),
            title: input.title,
            x: input.x,
            y: input.y,
            scale: input.scale,
        };
        let res = ctx.board_service.save_board(board_data.clone()).await;

        res.map(|_| board_data)
            .map_err(|e| juniper::FieldError::from(e))
    }

    async fn delete_board(
        ctx: &Context,
        board_id: juniper::ID,
        input: BoardInput,
    ) -> juniper::FieldResult<BoardData> {
        let board_data = BoardData {
            id: board_id.to_string(),
            board_id: board_id.to_string(),
            title: input.title,
            x: input.x,
            y: input.y,
            scale: input.scale,
        };
        let res = ctx
            .board_service
            .delete_board(BoardId::new(board_id.to_string()))
            .await;

        res.map(|_| BoardData {
            id: board_id.to_string(),
            ..board_data
        })
        .map_err(|e| juniper::FieldError::from(e))
    }

    async fn add_markdown(
        ctx: &Context,
        board_id: juniper::ID,
        input: MarkdownInput,
    ) -> juniper::FieldResult<BoxData> {
        add_box(ctx, board_id, BoxData::from(input)).await
    }

    async fn add_web_page(
        ctx: &Context,
        board_id: juniper::ID,
        input: WebPageInput,
    ) -> juniper::FieldResult<BoxData> {
        add_box(ctx, board_id, BoxData::from(input)).await
    }

    async fn add_image(
        ctx: &Context,
        board_id: juniper::ID,
        input: ImageInput,
    ) -> juniper::FieldResult<BoxData> {
        add_box(ctx, board_id, BoxData::from(input)).await
    }

    async fn add_pen(
        ctx: &Context,
        board_id: juniper::ID,
        input: PenInput,
    ) -> juniper::FieldResult<BoxData> {
        add_box(ctx, board_id, BoxData::from(input)).await
    }

    async fn add_square(
        ctx: &Context,
        board_id: juniper::ID,
        input: SquareInput,
    ) -> juniper::FieldResult<BoxData> {
        add_box(ctx, board_id, BoxData::from(input)).await
    }

    async fn add_arrow(
        ctx: &Context,
        board_id: juniper::ID,
        input: ArrowInput,
    ) -> juniper::FieldResult<BoxData> {
        add_box(ctx, board_id, BoxData::from(input)).await
    }

    async fn update_markdown(
        ctx: &Context,
        board_id: juniper::ID,
        box_id: juniper::ID,
        input: MarkdownInput,
    ) -> juniper::FieldResult<BoxData> {
        update_box(ctx, board_id, box_id, BoxData::from(input)).await
    }

    async fn update_web_page(
        ctx: &Context,
        board_id: juniper::ID,
        box_id: juniper::ID,
        input: WebPageInput,
    ) -> juniper::FieldResult<BoxData> {
        update_box(ctx, board_id, box_id, BoxData::from(input)).await
    }

    async fn update_image(
        ctx: &Context,
        board_id: juniper::ID,
        box_id: juniper::ID,
        input: ImageInput,
    ) -> juniper::FieldResult<BoxData> {
        update_box(ctx, board_id, box_id, BoxData::from(input)).await
    }

    async fn update_pen(
        ctx: &Context,
        board_id: juniper::ID,
        box_id: juniper::ID,
        input: PenInput,
    ) -> juniper::FieldResult<BoxData> {
        update_box(ctx, board_id, box_id, BoxData::from(input)).await
    }

    async fn update_square(
        ctx: &Context,
        board_id: juniper::ID,
        box_id: juniper::ID,
        input: SquareInput,
    ) -> juniper::FieldResult<BoxData> {
        update_box(ctx, board_id, box_id, BoxData::from(input)).await
    }

    async fn update_arrow(
        ctx: &Context,
        board_id: juniper::ID,
        box_id: juniper::ID,
        input: ArrowInput,
    ) -> juniper::FieldResult<BoxData> {
        update_box(ctx, board_id, box_id, BoxData::from(input)).await
    }

    async fn delete_markdown(
        ctx: &Context,
        box_id: juniper::ID,
        input: MarkdownInput,
    ) -> juniper::FieldResult<BoxData> {
        delete_box(ctx, box_id, BoxData::from(input)).await
    }

    async fn delete_web_page(
        ctx: &Context,
        box_id: juniper::ID,
        input: WebPageInput,
    ) -> juniper::FieldResult<BoxData> {
        delete_box(ctx, box_id, BoxData::from(input)).await
    }

    async fn delete_image(
        ctx: &Context,
        box_id: juniper::ID,
        input: ImageInput,
    ) -> juniper::FieldResult<BoxData> {
        delete_box(ctx, box_id, BoxData::from(input)).await
    }

    async fn delete_pen(
        ctx: &Context,
        box_id: juniper::ID,
        input: PenInput,
    ) -> juniper::FieldResult<BoxData> {
        delete_box(ctx, box_id, BoxData::from(input)).await
    }

    async fn delete_square(
        ctx: &Context,
        box_id: juniper::ID,
        input: SquareInput,
    ) -> juniper::FieldResult<BoxData> {
        delete_box(ctx, box_id, BoxData::from(input)).await
    }

    async fn delete_arrow(
        ctx: &Context,
        box_id: juniper::ID,
        input: ArrowInput,
    ) -> juniper::FieldResult<BoxData> {
        delete_box(ctx, box_id, BoxData::from(input)).await
    }
}
