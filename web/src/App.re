open Hooks;
open Belt;
[@bs.module] external uuidv4: unit => string = "uuid-random";

[@react.component]
let make = () => {
  let url = ReasonReact.Router.useUrl();
  let (pageId, _) = useSessionStorage(Constants.sessionPageId);

  switch (url.path) {
  | [] =>
    let pageId =
      pageId
      ->Option.flatMap(p => p == "" ? None : Some(p))
      ->Option.getWithDefault(uuidv4());
    let search = url.search;
    ReasonReactRouter.replace({j|/$(pageId)?$(search)|j});
    <div />;
  | [id]
      when
        switch (
          Js.Re.fromString(
            "[0-9a-fA-F]{8}\\-[0-9a-fA-F]{4}\\-[0-9a-fA-F]{4}\\-[0-9a-fA-F]{4}\\-[0-9a-fA-F]{12}",
          )
          ->Js.Re.exec_(id)
        ) {
        | Some(_) => true
        | None => false
        } =>
    <Board id />
  | _ => <NotFound />
  };
};
