let getMousePosition = (e: ReactEvent.Mouse.t) => (
  ReactEvent.Mouse.pageX(e),
  ReactEvent.Mouse.pageY(e),
);

let getTouchPosition = (e: ReactEvent.Touch.t) => (
  ReactEvent.Touch.touches(e)##item(0)##pageX,
  ReactEvent.Touch.touches(e)##item(0)##pageY,
);

module Promise = {
  let then_ = (promise, f) => Js.Promise.then_(f, promise);
  let ignore: Js.Promise.t(_) => unit = ignore;
};
