[@decco]
type t =
  | None
  | Arrow;

let toString = t => {
  switch (t) {
  | None => "none"
  | Arrow => "arrow"
  };
};

let fromString = s => {
  switch (s) {
  | "none" => None
  | "arrow" => Arrow
  | _ => Arrow
  };
};
