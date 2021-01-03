type user = {
  name: string,
  email: string,
  picture: string,
};

type getTokenSilentlyOptions = {
  audience: string,
  scope: string,
};

type idToken = {__raw: string};

type t = {
  loginWithRedirect: unit => Js.Promise.t(unit),
  isAuthenticated: bool,
  isLoading: bool,
  error: Js.Exn.t,
  logout: (~returnTo: string=?) => unit,
  user,
  getAccessTokenSilently: getTokenSilentlyOptions => Js.Promise.t(string),
  getIdTokenClaims: unit => Js.Promise.t(idToken),
};

[@bs.module "@auth0/auth0-react"] [@react.component]
external make:
  (
    ~domain: string,
    ~clientId: string,
    ~redirectUri: string,
    // ~scope: string,
    ~audience: string,
    // ~onRedirectCallback: (~returnTo: string) => unit,
    ~children: ReasonReact.reactElement
  ) =>
  React.element =
  "Auth0Provider";

[@bs.module "@auth0/auth0-react"] external useAuth0: unit => t = "useAuth0";
