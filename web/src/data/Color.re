[@decco]
type t = {
  red: int,
  green: int,
  blue: int,
  alpha: float,
};

let toString = ({red, green, blue, alpha}: t) => {j|rgba($red, $green, $blue, $alpha)|j};

let white = {red: 255, green: 255, blue: 255, alpha: 1.0};
let black = {red: 0, green: 0, blue: 0, alpha: 1.0};
let beige = {red: 255, green: 250, blue: 185, alpha: 1.0};
let gray = {red: 211, green: 211, blue: 211, alpha: 1.0};
let yellow = {red: 255, green: 249, blue: 178, alpha: 1.0};
let yellow2 = {red: 245, green: 209, blue: 49, alpha: 1.0};
let green = {red: 211, green: 248, blue: 226, alpha: 1.0};
let green2 = {red: 158, green: 214, blue: 131, alpha: 1.0};
let blue = {red: 206, green: 229, blue: 242, alpha: 1.0};
let blue2 = {red: 124, green: 146, blue: 252, alpha: 1.0};
let orange = {red: 247, green: 202, blue: 178, alpha: 1.0};
let pink = {red: 246, green: 207, blue: 230, alpha: 1.0};
let red = {red: 238, green: 138, blue: 139, alpha: 1.0};
let red2 = {red: 236, green: 123, blue: 139, alpha: 1.0};
let purple = {red: 205, green: 137, blue: 247, alpha: 1.0};
let transparent = {red: 0, green: 0, blue: 0, alpha: 0.0};

let colors = [
  transparent,
  white,
  black,
  beige,
  gray,
  yellow,
  yellow2,
  green,
  green2,
  blue,
  blue2,
  orange,
  pink,
  red,
  red2,
  purple,
];

let fromString = (c: string): t => {
  let tokens =
    Js.String2.replace(c, "rgba(", "")
    ->Js.String2.replace(")", "")
    ->Js.String2.trim
    ->Js.String2.split(",")
    ->Array.to_list;
  switch (tokens) {
  | [] => black
  | [r, g, b, a] => {
      red: int_of_string(Js.String2.trim(r)),
      green: int_of_string(Js.String2.trim(g)),
      blue: int_of_string(Js.String2.trim(b)),
      alpha: float_of_string(Js.String2.trim(a)),
    }
  | _ => black
  };
};
