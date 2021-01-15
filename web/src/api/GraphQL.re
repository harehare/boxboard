module BoardQuery = [%graphql
  {|
    query BoardQuery($id: String!) {
      board: board(id: $id) {
        id
        boxes {
          ... on Markdown {
            id
            x
            y
            width
            height
            order
            pinned
            color
            text
            fontSize
          }
          ...on WebPage {
            id
            x
            y
            width
            height
            order
            pinned
            url
            imageUrl
            title
            description
          }
          ...on Image {
            id
            x
            y
            width
            height
            order
            pinned
            image
          }
          ...on Pen {
            id
            x
            y
            width
            height
            order
            pinned
            drawList {
              color
              draw
            }
          }
          ...on Square {
            id
            x
            y
            width
            height
            order
            pinned
            color
          }
          ...on Arrow {
            id
            x
            y
            width
            height
            order
            pinned
            color
            arrowType
            angle
            strokeWidth
          }
        }
      }
    }
  |}
];

module AddMarkdown = [%graphql
  {|
    mutation addMarkdown($boardId: ID!, $input: MarkdownInput!) {
      board: addMarkdown(boardId: $boardId, input: $input) {
        ... on Markdown {
          id
        }
      }
    }
  |}
];

module AddWebPage = [%graphql
  {|
    mutation addWebPage($boardId: ID!, $input: WebPageInput!) {
      board: addWebPage(boardId: $boardId, input: $input) {
        ... on WebPage {
          id
        }
      }
    }
  |}
];

module AddImage = [%graphql
  {|
    mutation addImage($boardId: ID!, $input: ImageInput!) {
      board: addImage(boardId: $boardId, input: $input) {
        ... on Image {
          id
        }
      }
    }
  |}
];

module AddPen = [%graphql
  {|
    mutation addPen($boardId: ID!, $input: PenInput!) {
      board: addPen(boardId: $boardId, input: $input) {
        ... on Pen {
          id
        }
      }
    }
  |}
];

module AddSquare = [%graphql
  {|
    mutation addSquare($boardId: ID!, $input: SquareInput!) {
      board: addSquare(boardId: $boardId, input: $input) {
        ... on Square {
          id
        }
      }
    }
  |}
];

module AddArrow = [%graphql
  {|
    mutation addArrow($boardId: ID!, $input: ArrowInput!) {
      board: addArrow(boardId: $boardId, input: $input) {
        ... on Arrow {
          id
        }
      }
    }
  |}
];

module UpdateMarkdown = [%graphql
  {|
    mutation updateMarkdown($boardId: ID!, $boxId: ID!, $input: MarkdownInput!) {
      board: updateMarkdown(boardId: $boardId, boxId: $boxId, input: $input) {
        ... on Markdown {
          id
        }
      }
    }
  |}
];

module UpdateWebPage = [%graphql
  {|
    mutation updateWebPage($boardId: ID!, $boxId: ID!, $input: WebPageInput!) {
      board: updateWebPage(boardId: $boardId, boxId: $boxId, input: $input) {
        ... on WebPage {
          id
        }
      }
    }
  |}
];

module UpdateImage = [%graphql
  {|
    mutation updateImage($boardId: ID!, $boxId: ID!, $input: ImageInput!) {
      board: updateImage(boardId: $boardId, boxId: $boxId, input: $input) {
        ... on Image {
          id
        }
      }
    }
  |}
];

module UpdatePen = [%graphql
  {|
    mutation updatePen($boardId: ID!, $boxId: ID!, $input: PenInput!) {
      board: updatePen(boardId: $boardId, boxId: $boxId, input: $input) {
        ... on Pen {
          id
        }
      }
    }
  |}
];

module UpdateSquare = [%graphql
  {|
    mutation updateSquare($boardId: ID!, $boxId: ID!, $input: SquareInput!) {
      board: updateSquare(boardId: $boardId, boxId: $boxId, input: $input) {
        ... on Square {
          id
        }
      }
    }
  |}
];

module UpdateArrow = [%graphql
  {|
    mutation updateArrow($boardId: ID!, $boxId: ID!, $input: ArrowInput!) {
      board: updateArrow(boardId: $boardId, boxId: $boxId, input: $input) {
        ... on Arrow {
          id
        }
      }
    }
  |}
];

module DeleteMarkdown = [%graphql
  {|
    mutation deleteMarkdown($boxId: ID!, $input: MarkdownInput!) {
      board: deleteMarkdown(boxId: $boxId, input: $input) {
        ... on Markdown {
          id
        }
      }
    }
  |}
];

module DeleteWebPage = [%graphql
  {|
    mutation deleteWebPage($boxId: ID!, $input: WebPageInput!) {
      board: deleteWebPage(boxId: $boxId, input: $input) {
        ... on WebPage {
          id
        }
      }
    }
  |}
];

module DeleteImage = [%graphql
  {|
    mutation deleteImage($boxId: ID!, $input: ImageInput!) {
      board: deleteImage(boxId: $boxId, input: $input) {
        ... on Image {
          id
        }
      }
    }
  |}
];

module DeletePen = [%graphql
  {|
    mutation deletePen($boxId: ID!, $input: PenInput!) {
      board: deletePen(boxId: $boxId, input: $input) {
        ... on Pen {
          id
        }
      }
    }
  |}
];

module DeleteSquare = [%graphql
  {|
    mutation deleteSquare($boxId: ID!, $input: SquareInput!) {
      board: deleteSquare(boxId: $boxId, input: $input) {
        ... on Square {
          id
        }
      }
    }
  |}
];

module DeleteArrow = [%graphql
  {|
    mutation deleteArrow($boxId: ID!, $input: ArrowInput!) {
      board: deleteArrow(boxId: $boxId, input: $input) {
        ... on Arrow {
          id
        }
      }
    }
  |}
];

module AddInput = {
  let toMarkdownInput = (box: Box.t): AddMarkdown.t_variables_MarkdownInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let (text, color, fontSize) =
      switch (box.kind) {
      | Markdown(text, color, fontSize) => (text, color, fontSize)
      | _ => ("", Color.transparent, FontSize.default)
      };
    AddMarkdown.makeInputObjectMarkdownInput(
      ~id=box.id,
      ~x,
      ~y,
      ~width,
      ~height,
      ~order=box.order,
      ~pinned=box.pinned,
      ~color=Color.toString(color),
      ~text,
      ~fontSize,
      (),
    );
  };

  let toPenInput = (box: Box.t): AddPen.t_variables_PenInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let drawList =
      switch (box.kind) {
      | Pen(_, _, drawList, _) =>
        Array.of_list(
          List.map(
            (d: Box.draw) =>
              AddPen.makeInputObjectPenDrawInput(
                ~draw=d.d,
                ~color=Color.toString(d.color),
                (),
              ),
            drawList,
          ),
        )
      | _ => Array.of_list([])
      };
    AddPen.makeInputObjectPenInput(
      ~id=box.id,
      ~x,
      ~y,
      ~width,
      ~height,
      ~order=box.order,
      ~pinned=box.pinned,
      ~drawList,
      (),
    );
  };

  let toWebPageInput = (box: Box.t): AddWebPage.t_variables_WebPageInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let (url, webPage) =
      switch (box.kind) {
      | Web(url, page) => (url, page)
      | _ => ("", None)
      };
    switch (webPage) {
    | Some(page) =>
      AddWebPage.makeInputObjectWebPageInput(
        ~id=box.id,
        ~x,
        ~y,
        ~width,
        ~height,
        ~order=box.order,
        ~pinned=box.pinned,
        ~url,
        ~imageUrl=page.imageUrl->Belt.Option.getWithDefault(""),
        ~title=page.title->Belt.Option.getWithDefault(""),
        ~description=page.description->Belt.Option.getWithDefault(""),
        (),
      )
    | None =>
      AddWebPage.makeInputObjectWebPageInput(
        ~id=box.id,
        ~x,
        ~y,
        ~width,
        ~height,
        ~order=box.order,
        ~pinned=box.pinned,
        ~url,
        ~imageUrl="",
        ~title="",
        ~description="",
        (),
      )
    };
  };

  let toImageInput = (box: Box.t): AddImage.t_variables_ImageInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let image =
      switch (box.kind) {
      | Image(image) => image
      | _ => None
      };
    AddImage.makeInputObjectImageInput(
      ~id=box.id,
      ~x,
      ~y,
      ~width,
      ~height,
      ~order=box.order,
      ~pinned=box.pinned,
      ~image=Belt.Option.getWithDefault(image, ""),
      (),
    );
  };

  let toSquareInput = (box: Box.t): AddSquare.t_variables_SquareInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let color =
      switch (box.kind) {
      | Square(color) => color
      | _ => Color.black
      };
    AddSquare.makeInputObjectSquareInput(
      ~id=box.id,
      ~x,
      ~y,
      ~width,
      ~height,
      ~order=box.order,
      ~pinned=box.pinned,
      ~color=color->Color.toString,
      (),
    );
  };

  let toArrowInput = (box: Box.t): AddArrow.t_variables_ArrowInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let (color, arrowType, angle, strokeWidth) =
      switch (box.kind) {
      | Arrow(color, arrowType, angle, strokeWidth) => (
          color,
          arrowType,
          angle,
          strokeWidth,
        )
      | _ => (Color.black, ArrowType.Arrow, 0, 2)
      };
    AddArrow.makeInputObjectArrowInput(
      ~id=box.id,
      ~x,
      ~y,
      ~width,
      ~height,
      ~order=box.order,
      ~pinned=box.pinned,
      ~color=color->Color.toString,
      ~arrowType=arrowType->ArrowType.toString,
      ~angle,
      ~strokeWidth,
      (),
    );
  };
};

module UpdateInput = {
  let toMarkdownInput = (box: Box.t): UpdateMarkdown.t_variables_MarkdownInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let (text, color, fontSize) =
      switch (box.kind) {
      | Markdown(text, color, fontSize) => (text, color, fontSize)
      | _ => ("", Color.transparent, FontSize.default)
      };
    UpdateMarkdown.makeInputObjectMarkdownInput(
      ~id=box.id,
      ~x,
      ~y,
      ~width,
      ~height,
      ~order=box.order,
      ~pinned=box.pinned,
      ~color=Color.toString(color),
      ~text,
      ~fontSize,
      (),
    );
  };

  let toPenInput = (box: Box.t): UpdatePen.t_variables_PenInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let drawList =
      switch (box.kind) {
      | Pen(_, _, drawList, _) =>
        Array.of_list(
          List.map(
            (d: Box.draw) =>
              UpdatePen.makeInputObjectPenDrawInput(
                ~draw=d.d,
                ~color=Color.toString(d.color),
                (),
              ),
            drawList,
          ),
        )
      | _ => Array.of_list([])
      };
    UpdatePen.makeInputObjectPenInput(
      ~id=box.id,
      ~x,
      ~y,
      ~width,
      ~height,
      ~order=box.order,
      ~pinned=box.pinned,
      ~drawList,
      (),
    );
  };

  let toWebPageInput = (box: Box.t): UpdateWebPage.t_variables_WebPageInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let (url, webPage) =
      switch (box.kind) {
      | Web(url, page) => (url, page)
      | _ => ("", None)
      };
    switch (webPage) {
    | Some(page) =>
      UpdateWebPage.makeInputObjectWebPageInput(
        ~id=box.id,
        ~x,
        ~y,
        ~width,
        ~height,
        ~order=box.order,
        ~pinned=box.pinned,
        ~url,
        ~imageUrl=page.imageUrl->Belt.Option.getWithDefault(""),
        ~title=page.title->Belt.Option.getWithDefault(""),
        ~description=page.description->Belt.Option.getWithDefault(""),
        (),
      )
    | None =>
      UpdateWebPage.makeInputObjectWebPageInput(
        ~id=box.id,
        ~x,
        ~y,
        ~width,
        ~height,
        ~order=box.order,
        ~pinned=box.pinned,
        ~url,
        ~imageUrl="",
        ~title="",
        ~description="",
        (),
      )
    };
  };

  let toImageInput = (box: Box.t): UpdateImage.t_variables_ImageInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let image =
      switch (box.kind) {
      | Image(image) => image
      | _ => None
      };
    UpdateImage.makeInputObjectImageInput(
      ~id=box.id,
      ~x,
      ~y,
      ~width,
      ~height,
      ~order=box.order,
      ~pinned=box.pinned,
      ~image=Belt.Option.getWithDefault(image, ""),
      (),
    );
  };

  let toSquareInput = (box: Box.t): UpdateSquare.t_variables_SquareInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let color =
      switch (box.kind) {
      | Square(color) => color
      | _ => Color.black
      };
    UpdateSquare.makeInputObjectSquareInput(
      ~id=box.id,
      ~x,
      ~y,
      ~width,
      ~height,
      ~order=box.order,
      ~pinned=box.pinned,
      ~color=color->Color.toString,
      (),
    );
  };

  let toArrowInput = (box: Box.t): UpdateArrow.t_variables_ArrowInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let (color, arrowType, angle, strokeWidth) =
      switch (box.kind) {
      | Arrow(color, arrowType, angle, strokeWdith) => (
          color,
          arrowType,
          angle,
          strokeWdith,
        )
      | _ => (Color.black, ArrowType.Arrow, 0, 2)
      };
    UpdateArrow.makeInputObjectArrowInput(
      ~id=box.id,
      ~x,
      ~y,
      ~width,
      ~height,
      ~order=box.order,
      ~pinned=box.pinned,
      ~color=color->Color.toString,
      ~arrowType=arrowType->ArrowType.toString,
      ~angle,
      ~strokeWidth,
      (),
    );
  };
};

module DeleteInput = {
  let toMarkdownInput = (box: Box.t): DeleteMarkdown.t_variables_MarkdownInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let (text, color, fontSize) =
      switch (box.kind) {
      | Markdown(text, color, fontSize) => (text, color, fontSize)
      | _ => ("", Color.transparent, FontSize.default)
      };
    DeleteMarkdown.makeInputObjectMarkdownInput(
      ~id=box.id,
      ~x,
      ~y,
      ~width,
      ~height,
      ~order=box.order,
      ~pinned=box.pinned,
      ~color=Color.toString(color),
      ~text,
      ~fontSize,
      (),
    );
  };

  let toPenInput = (box: Box.t): DeletePen.t_variables_PenInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let drawList =
      switch (box.kind) {
      | Pen(_, _, drawList, _) =>
        Array.of_list(
          List.map(
            (d: Box.draw) =>
              DeletePen.makeInputObjectPenDrawInput(
                ~draw=d.d,
                ~color=Color.toString(d.color),
                (),
              ),
            drawList,
          ),
        )
      | _ => Array.of_list([])
      };
    DeletePen.makeInputObjectPenInput(
      ~id=box.id,
      ~x,
      ~y,
      ~width,
      ~height,
      ~order=box.order,
      ~pinned=box.pinned,
      ~drawList,
      (),
    );
  };

  let toWebPageInput = (box: Box.t): DeleteWebPage.t_variables_WebPageInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let (url, webPage) =
      switch (box.kind) {
      | Web(url, page) => (url, page)
      | _ => ("", None)
      };
    switch (webPage) {
    | Some(page) =>
      DeleteWebPage.makeInputObjectWebPageInput(
        ~id=box.id,
        ~x,
        ~y,
        ~width,
        ~height,
        ~order=box.order,
        ~pinned=box.pinned,
        ~url,
        ~imageUrl=page.imageUrl->Belt.Option.getWithDefault(""),
        ~title=page.title->Belt.Option.getWithDefault(""),
        ~description=page.description->Belt.Option.getWithDefault(""),
        (),
      )
    | None =>
      DeleteWebPage.makeInputObjectWebPageInput(
        ~id=box.id,
        ~x,
        ~y,
        ~width,
        ~height,
        ~order=box.order,
        ~pinned=box.pinned,
        ~url,
        ~imageUrl="",
        ~title="",
        ~description="",
        (),
      )
    };
  };

  let toImageInput = (box: Box.t): DeleteImage.t_variables_ImageInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let image =
      switch (box.kind) {
      | Image(image) => image
      | _ => None
      };
    DeleteImage.makeInputObjectImageInput(
      ~id=box.id,
      ~x,
      ~y,
      ~width,
      ~height,
      ~order=box.order,
      ~pinned=box.pinned,
      ~image=Belt.Option.getWithDefault(image, ""),
      (),
    );
  };

  let toSquareInput = (box: Box.t): DeleteSquare.t_variables_SquareInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let color =
      switch (box.kind) {
      | Square(color) => color
      | _ => Color.black
      };
    DeleteSquare.makeInputObjectSquareInput(
      ~id=box.id,
      ~x,
      ~y,
      ~width,
      ~height,
      ~order=box.order,
      ~pinned=box.pinned,
      ~color=color->Color.toString,
      (),
    );
  };

  let toArrowInput = (box: Box.t): DeleteArrow.t_variables_ArrowInput => {
    let (x, y) = box.position;
    let (width, height) = box.size;
    let (color, arrowType, angle, strokeWidth) =
      switch (box.kind) {
      | Arrow(_, arrowType, angle, strokeWidth) => (
          Color.black,
          arrowType,
          angle,
          strokeWidth,
        )
      | _ => (Color.black, ArrowType.Arrow, 0, 2)
      };
    DeleteArrow.makeInputObjectArrowInput(
      ~id=box.id,
      ~x,
      ~y,
      ~width,
      ~height,
      ~order=box.order,
      ~pinned=box.pinned,
      ~color=color->Color.toString,
      ~arrowType=arrowType->ArrowType.toString,
      ~angle,
      ~strokeWidth,
      (),
    );
  };
};

let toBox = (src: BoardQuery.t_board_boxes): Box.t => {
  switch (src) {
  | `Markdown(markdown) => {
      id: markdown.id,
      position: (markdown.x, markdown.y),
      movePosition: (0, 0),
      size: (markdown.width, markdown.height),
      kind:
        Markdown(
          markdown.text,
          Color.fromString(markdown.color),
          markdown.fontSize == 0 ? FontSize.default : markdown.fontSize,
        ),
      status: Box.None,
      loading: false,
      order: markdown.order,
      isRemote: true,
      pinned: markdown.pinned,
    }
  | `WebPage(web) => {
      id: web.id,
      position: (web.x, web.y),
      movePosition: (0, 0),
      size: (web.width, web.height),
      kind:
        Web(
          web.url,
          Some({
            imageUrl: web.imageUrl,
            title: web.title,
            description: web.description,
          }),
        ),
      status: Box.None,
      loading: false,
      order: web.order,
      isRemote: true,
      pinned: web.pinned,
    }
  | `Image(image) => {
      id: image.id,
      position: (image.x, image.y),
      movePosition: (0, 0),
      size: (image.width, image.height),
      kind: Image(Some(image.image)),
      status: Box.None,
      loading: false,
      order: image.order,
      isRemote: true,
      pinned: image.pinned,
    }
  | `Pen(pen) => {
      id: pen.id,
      position: (pen.x, pen.y),
      movePosition: (0, 0),
      size: (pen.width, pen.height),
      kind:
        Pen(
          [],
          {id: None, d: "", color: Color.black},
          Array.map(
            (d: BoardQuery.t_board_boxes_Pen_drawList) => {
              let r: Box.draw = {
                id: None,
                color: Color.fromString(d.color),
                d: d.draw,
              };
              r;
            },
            pen.drawList,
          )
          ->Array.to_list,
          false,
        ),
      status: Box.None,
      loading: false,
      order: pen.order,
      isRemote: true,
      pinned: pen.pinned,
    }
  | `Square(square) => {
      id: square.id,
      position: (square.x, square.y),
      movePosition: (0, 0),
      size: (square.width, square.height),
      kind: Square(Color.fromString(square.color)),
      status: Box.None,
      loading: false,
      order: square.order,
      isRemote: true,
      pinned: square.pinned,
    }
  | `Arrow(arrow) => {
      id: arrow.id,
      position: (arrow.x, arrow.y),
      movePosition: (0, 0),
      size: (arrow.width, arrow.height),
      kind:
        Arrow(
          Color.fromString(arrow.color),
          arrow.arrowType->ArrowType.fromString,
          arrow.angle,
          arrow.strokeWidth,
        ),
      status: Box.None,
      loading: false,
      order: arrow.order,
      isRemote: true,
      pinned: arrow.pinned,
    }
  | `FutureAddedValue(_) => {
      id: "",
      position: (0, 0),
      movePosition: (0, 0),
      size: (0, 0),
      kind: Error("undefined kind"),
      status: Box.None,
      loading: false,
      order: 0,
      isRemote: true,
      pinned: false,
    }
  };
};
