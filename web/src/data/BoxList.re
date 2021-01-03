module StringCmp =
  Belt.Id.MakeComparable({
    type t = string;
    let cmp = (a, b) => Pervasives.compare(a, b);
  });

type t = Belt.Map.t(StringCmp.t, Box.t, StringCmp.identity);

let empty: t = Belt.Map.fromArray([||], ~id=(module StringCmp));

let isEmpty = (boxList: t) => {
  boxList->Belt.Map.isEmpty;
};

let get = (boxList: t, id: string): option(Box.t) => {
  boxList->Belt.Map.get(id);
};

let set = (boxList: t, addBox: Box.t): t => {
  boxList->Belt.Map.set(addBox.id, addBox);
};

let remove = (boxList: t, id: string) => {
  boxList->Belt.Map.remove(id);
};

let merge = (src: t, dest: t) => {
  Belt.Map.merge(src, dest, (_, box1, box2) => {
    switch (box1, box2) {
    | (Some(_), Some(b2)) => Some(b2)
    | (Some(b1), None) => Some(b1)
    | (None, Some(b2)) => Some(b2)
    | _ => None
    }
  });
};

let update = (boxList: t, newId: string, updateBox: Box.t => Box.t): t => {
  boxList->Belt.Map.update(newId, (box: option(Box.t)) =>
    (
      switch (box) {
      | Some(b) => Some(updateBox(b))
      | None => None
      }:
        option(Box.t)
    )
  );
};

let fromList = (boxes: list(Box.t)): t => {
  List.map((b: Box.t) => (b.id, b), boxes)
  ->Array.of_list
  ->Belt.Map.fromArray(~id=(module StringCmp));
};

let toList = (boxList: t): list(Box.t) => {
  List.sort(
    (a: Box.t, b: Box.t) => a.order - b.order,
    boxList->Belt.Map.valuesToArray->Array.to_list,
  );
};

let size = (boxList: t): Box.size => {
  List.fold_left(
    (b1: (int, int), b2: (int, int)) => {
      let (w1, h1) = b1;
      let (w2, h2) = b2;
      (max(w1, w2), max(h1, h2));
    },
    (0, 0),
    List.map(
      (b: Box.t) => {
        let (width, height) = b.size;
        let (x, y) = b.position;
        (x + width, y + height);
      },
      boxList->toList,
    ),
  );
};
