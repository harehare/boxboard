open Env;
[%bs.raw {|require('./Index.scss')|}];
let window = [%bs.raw "window"];

ReactDOMRe.renderToElementWithId(
  <Auth0 domain clientId audience redirectUri=window##location##origin>
    <AppStore.Provider store=AppStore.appStore>
      <ApolloClient.React.ApolloProvider client=Client.instance>
        <App />
      </ApolloClient.React.ApolloProvider>
    </AppStore.Provider>
  </Auth0>,
  "root",
);

%bs.raw
{|
if (
    "serviceWorker" in navigator &&
    !window.location.host.startsWith("localhost")
) {
    navigator.serviceWorker.register("/sw.js");
}
|};
