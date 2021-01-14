[@bs.val] external window: Js.t({..}) = "window";

let screenSize = () => {
  (window##parent##screen##width, window##parent##screen##height);
};

let getMousePosition = (e: ReactEvent.Mouse.t) => (
  ReactEvent.Mouse.pageX(e),
  ReactEvent.Mouse.pageY(e),
);

let getClientMousePosition = (e: ReactEvent.Mouse.t) => (
  ReactEvent.Mouse.clientX(e),
  ReactEvent.Mouse.clientY(e),
);

let getTouchPosition = (e: ReactEvent.Touch.t) => (
  ReactEvent.Touch.touches(e)##item(0)##pageX,
  ReactEvent.Touch.touches(e)##item(0)##pageY,
);

let getClientTouchPosition = (e: ReactEvent.Touch.t) => (
  ReactEvent.Touch.touches(e)##item(0)##clientX,
  ReactEvent.Touch.touches(e)##item(0)##clientY,
);

module Promise = {
  let then_ = (promise, f) => Js.Promise.then_(f, promise);
  let ignore: Js.Promise.t(_) => unit = ignore;
};
