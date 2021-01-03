use crate::domain::board::model::Board;
use crate::domain::values::board_id::BoardId;
use crate::domain::values::user_id::UserId;
use crate::graphql::context::Context;

pub struct QueryRoot;

#[juniper::graphql_object(Context = Context)]
impl QueryRoot {
    async fn board(ctx: &Context, id: String) -> juniper::FieldResult<Board> {
        let res = ctx
            .board_service
            .get_board(BoardId::new(id), UserId::new(ctx.uid.clone()))
            .await;
        res.map_err(|e| juniper::FieldError::from(e))
    }
}
