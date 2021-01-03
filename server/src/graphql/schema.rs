use crate::graphql::context::Context;
use crate::graphql::mutation::MutationRoot;
use crate::graphql::query::QueryRoot;
use juniper::EmptySubscription;

pub type Schema = juniper::RootNode<'static, QueryRoot, MutationRoot, EmptySubscription<Context>>;

pub fn schema() -> Schema {
    Schema::new(QueryRoot, MutationRoot, EmptySubscription::<Context>::new())
}
