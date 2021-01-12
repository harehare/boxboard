type element;
let window: element = [%bs.raw "window"];

[@bs.send]
external addEventListener:
  (element, string, ReactEvent.Keyboard.t => unit) => unit =
  "addEventListener";

[@bs.send]
external removeEventListener:
  (element, string, ReactEvent.Keyboard.t => unit) => unit =
  "removeEventListener";

[@bs.get] external getWidth: element => int = "innerWidth";
[@bs.get] external getHeight: element => int = "innerHeight";
