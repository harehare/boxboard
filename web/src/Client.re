let httpLink =
  ApolloClient.Link.HttpLink.make(
    ~uri=_ => Env.apiUrl ++ "/graphql",
    ~credentials="include",
    (),
  );

let authLink =
  ApolloClient.Link.ContextLink.make(
    (
      ~operation: ApolloClient__Link_Context.GraphQLRequest.t,
      ~prevContext: Js.Json.t,
    ) => {
    let token =
      Dom.Storage.getItem(Constants.sessionToken, Dom.Storage.sessionStorage);
    let headers = {
      "headers": {
        "Authorization": {j|Bearer $token|j},
      },
    };
    Obj.magic(headers);
  });

let instance =
  ApolloClient.(
    make(
      ~cache=Cache.InMemoryCache.make(),
      ~connectToDevTools=true,
      ~defaultOptions=
        DefaultOptions.make(
          ~mutate=
            DefaultMutateOptions.make(
              ~awaitRefetchQueries=true,
              ~errorPolicy=All,
              (),
            ),
          ~query=
            DefaultQueryOptions.make(
              ~fetchPolicy=NetworkOnly,
              ~errorPolicy=All,
              (),
            ),
          (),
        ),
      ~link=ApolloClient.Link.concat(authLink, httpLink),
      (),
    )
  );
