type t =
  | UnknownError
  | NetworkError(string)
  | GraphQLError(string);

let toString = err => {
  switch (err) {
  | UnknownError => "Unknown error"
  | NetworkError(e) => {j|$e error|j}
  | GraphQLError(e) => {j|GraphQL error $e|j}
  };
};

let fromApolloError = (err: ApolloClient__ApolloClient.ApolloError.t) => {
  switch (err.graphQLErrors, err.networkError) {
  | (errors, None) =>
    GraphQLError(
      Array.fold_left(
        (a, b) => a ++ "," ++ b,
        "",
        Array.map(
          (
            e: ApolloClient__ApolloClient.ApolloError.Graphql.Error.GraphQLError.t,
          ) =>
            e.message,
          errors,
        ),
      ),
    )
  | (_, Some(networkError)) =>
    switch (networkError) {
    | FetchFailure(_) => NetworkError("Fetch Failure")
    | BadStatus(status, _) => NetworkError({j|Bad Status $status|j})
    | BadBody(err) => NetworkError(err.bodyText)
    | ParseError(_) => NetworkError("")
    }
  };
};
