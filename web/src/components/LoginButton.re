open AppStore;

[%bs.raw {|require('./LoginButton.scss')|}];
let window = [%bs.raw "window"];
let idSelector = state => state.id;

[@react.component]
let make = () => {
  let {isAuthenticated, user, logout, loginWithRedirect}: Auth0.t =
    Auth0.useAuth0();
  let boardId = useSelector(idSelector);

  if (isAuthenticated) {
    <div onClick={_ => {logout(~returnTo=window##location##origin)}}>
      <img src={user.picture} alt="user-picture" className="user-picture" />
    </div>;
  } else {
    <div
      className="login-button"
      onClick={_ => {
        Dom.Storage.setItem(
          Constants.sessionPageId,
          boardId,
          Dom.Storage.sessionStorage,
        )
        |> ignore;
        loginWithRedirect() |> ignore;
      }}>
      "Login / Sign up"->React.string
    </div>;
  };
};
