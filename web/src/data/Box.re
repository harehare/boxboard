[@bs.module] external uuidv4: unit => string = "uuid-random";
[@decco]
type text = string;
[@decco]
type url = string;
[@decco]
type page = {
  imageUrl: option(string),
  title: option(string),
  description: option(string),
};
[@decco]
type coordinates('a) = ('a, 'a);
[@decco]
type position = coordinates(int);
[@decco]
type size = coordinates(int);
[@decco]
type isEdited = bool;
[@decco]
type draw = {
  id: option(string),
  d: string,
  color: Color.t,
};
[@decco]
type fontSize = int;

[@decco]
type kind =
  | Markdown(text, Color.t, fontSize)
  | Web(url, option(page))
  | Image(option(string))
  | Pen(list(position), draw, list(draw), isEdited)
  | Square(Color.t)
  | Arrow(Color.t, ArrowType.t, int)
  | Error(string);

[@decco]
type status =
  | None
  | Move
  | ResizeTopLeft
  | ResizeTopRight
  | ResizeBottomLeft
  | ResizeBottomRight;

[@decco]
type t = {
  id: string,
  position,
  movePosition: position,
  size,
  kind,
  status,
  pinned: bool,
  loading: bool,
  isRemote: bool,
  order: int,
};

let equals = (b1: t, b2: t): bool => {
  b1.id == b2.id
  && b1.position == b2.position
  && b1.size == b2.size
  && b1.kind == b2.kind
  && b1.pinned == b2.pinned
  && b1.order == b2.order;
};

let newBox = (~kind, ~position=(0, 0), ()) => {
  id: uuidv4(),
  position,
  movePosition: (0, 0),
  size: (192, 192),
  kind,
  status: None,
  pinned: false,
  loading: false,
  isRemote: false,
  order: 0,
};

let empty = {
  id: "",
  position: (0, 0),
  movePosition: (0, 0),
  size: (192, 192),
  kind: Square(Color.black),
  status: None,
  pinned: false,
  loading: false,
  isRemote: false,
  order: 0,
};

[@decco]
type boxData = {
  id: string,
  boxes: list(t),
  position,
  scale: float,
};

let drawPoints = (points: list(position)) => {
  switch (points) {
  | [head] =>
    let (p1, p2) = head;
    {j|M$(p1) $p2|j};
  | [head, ...tail] =>
    let (x1, y1) = head;
    let points =
      List.map(
        path => {
          let (x, y) = path;
          {j|L$(x) $y|j};
        },
        tail,
      )
      |> List.fold_left((a, b) => {j|$a $b|j}, "");
    {j|M$(x1) $y1 $points|j};
  | _ => ""
  };
};

let addPenPoints = (points: list(position), position, draw, drawList) => {
  switch (points) {
  | [head, ..._] =>
    let (headX, headY) = head;
    let (newX, newY) = position;

    if (headX == newX && headY == newY) {
      Pen(points, draw, drawList, false);
    } else {
      let newPoints = [position, ...points];
      Pen(newPoints, {...draw, d: drawPoints(newPoints)}, drawList, false);
    };
  | _ =>
    let newPoints = [position, ...points];
    Pen(newPoints, {...draw, d: drawPoints(newPoints)}, drawList, false);
  };
};
