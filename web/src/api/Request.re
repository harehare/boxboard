open GraphQL;

let findBoxesInBoard = id =>
  Client.instance.query(~query=(module BoardQuery), {id: id})
  ->Utils.Promise.then_(result => {
      Js.Promise.resolve(
        switch (result) {
        | Ok({data: {board}}) =>
          Ok(Array.map(toBox, board.boxes)->Array.to_list)
        | Error(error) => Error(Err.fromApolloError(error))
        },
      )
    });

let addMarkdown = (boardId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module AddMarkdown),
    {boardId, input: AddInput.toMarkdownInput(box)},
  )
  ->Utils.Promise.then_(result => {
      result
      ->Belt.Result.map(r => {
          switch (r.data.board) {
          | `Markdown(markdown) => markdown.id
          | _ => ""
          }
        })
      ->Js.Promise.resolve
    });

let addWebPage = (boardId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module AddWebPage),
    {boardId, input: AddInput.toWebPageInput(box)},
  )
  ->Utils.Promise.then_(result => {
      result
      ->Belt.Result.map(r => {
          switch (r.data.board) {
          | `WebPage(web) => web.id
          | _ => ""
          }
        })
      ->Js.Promise.resolve
    });

let addImage = (boardId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module AddImage),
    {boardId, input: AddInput.toImageInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result
      ->Belt.Result.map(r => {
          switch (r.data.board) {
          | `Image(image) => image.id
          | _ => ""
          }
        })
      ->Js.Promise.resolve
    );

let addPen = (boardId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module AddPen),
    {boardId, input: AddInput.toPenInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result
      ->Belt.Result.map(r => {
          switch (r.data.board) {
          | `Pen(pen) => pen.id
          | _ => ""
          }
        })
      ->Js.Promise.resolve
    );

let addSquare = (boardId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module AddSquare),
    {boardId, input: AddInput.toSquareInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result
      ->Belt.Result.map(r => {
          switch (r.data.board) {
          | `Square(square) => square.id
          | _ => ""
          }
        })
      ->Js.Promise.resolve
    );

let addArrow = (boardId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module AddArrow),
    {boardId, input: AddInput.toArrowInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result
      ->Belt.Result.map(r => {
          switch (r.data.board) {
          | `Arrow(arrow) => arrow.id
          | _ => ""
          }
        })
      ->Js.Promise.resolve
    );

let updateMarkdown = (boardId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module UpdateMarkdown),
    {boardId, boxId: box.id, input: UpdateInput.toMarkdownInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result->Belt.Result.map(_ => box.id)->Js.Promise.resolve
    );

let updateWebPage = (boardId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module UpdateWebPage),
    {boardId, boxId: box.id, input: UpdateInput.toWebPageInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result->Belt.Result.map(_ => box.id)->Js.Promise.resolve
    );

let updateImage = (boardId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module UpdateImage),
    {boardId, boxId: box.id, input: UpdateInput.toImageInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result->Belt.Result.map(_ => box.id)->Js.Promise.resolve
    );

let updatePen = (boardId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module UpdatePen),
    {boardId, boxId: box.id, input: UpdateInput.toPenInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result->Belt.Result.map(_ => box.id)->Js.Promise.resolve
    );

let updateSquare = (boardId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module UpdateSquare),
    {boardId, boxId: box.id, input: UpdateInput.toSquareInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result->Belt.Result.map(_ => box.id)->Js.Promise.resolve
    );

let updateArrow = (boardId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module UpdateArrow),
    {boardId, boxId: box.id, input: UpdateInput.toArrowInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result->Belt.Result.map(_ => box.id)->Js.Promise.resolve
    );

let deleteMarkdown = (boxId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module DeleteMarkdown),
    {boxId, input: DeleteInput.toMarkdownInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result->Belt.Result.map(_ => box.id)->Js.Promise.resolve
    );

let deleteWebPage = (boxId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module DeleteWebPage),
    {boxId, input: DeleteInput.toWebPageInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result->Belt.Result.map(_ => box.id)->Js.Promise.resolve
    );

let deleteImage = (boxId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module DeleteImage),
    {boxId, input: DeleteInput.toImageInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result->Belt.Result.map(_ => box.id)->Js.Promise.resolve
    );

let deletePen = (boxId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module DeletePen),
    {boxId, input: DeleteInput.toPenInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result->Belt.Result.map(_ => box.id)->Js.Promise.resolve
    );

let deleteSquare = (boxId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module DeleteSquare),
    {boxId, input: DeleteInput.toSquareInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result->Belt.Result.map(_ => box.id)->Js.Promise.resolve
    );

let deleteArrow = (boxId: string, box: Box.t) =>
  Client.instance.mutate(
    ~mutation=(module DeleteArrow),
    {boxId, input: DeleteInput.toArrowInput(box)},
  )
  ->Utils.Promise.then_(result =>
      result->Belt.Result.map(_ => box.id)->Js.Promise.resolve
    );
