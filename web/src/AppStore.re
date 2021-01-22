open Env;
[@bs.val] external encodeURIComponent: string => string = "encodeURIComponent";

type error = string;

type remoteData = RemoteData.t(BoxList.t, BoxList.t, error);

type fileType = {
  .
  lastModified: int,
  lastModifiedDate: string,
  name: string,
  size: int,
  type_: string,
  webkitRelativePath: string,
};

type boardAction =
  | Init(string)
  | EditTitle(string)
  | UpdateBoxId(string, string)
  | UpdateBoardId(string)
  | LoadingBoard
  | LoadedBoard(Request.boardResponse)
  | Error(error)
  | Cursor
  | Adding(Box.kind)
  | Add(Box.t)
  | Select(Box.t)
  | UnSelect(Box.t)
  | StartMoveBoard(Box.position)
  | MoveBoard(Box.position)
  | StopMoveBoard
  | Search(option(string))
  | ZoomIn
  | ZoomOut
  | Undo
  | Redo;

type boxAction =
  | Loading(bool)
  | StartMove(Box.position, Box.status)
  | Move(Box.position)
  | StopMove
  | Remove(Box.t)
  | BringToFront
  | SendBackward
  | StartResize(Box.position, Box.status)
  | EditMarkdown(string)
  | EditUrl(string)
  | ColorChanged(Color.t)
  | FontSizeChanged(int)
  | ArrowTypeChanged(ArrowType.t)
  | StrokeWidthChanged(int)
  | LoadUrl(string)
  | LoadUrlComplete(Belt.Result.t(Box.page, string))
  | LoadFile(Box.kind, string)
  | Pinned(bool)
  | Copy(Box.t)
  | LoadFileEvent(Box.kind, FileReader.File.t);

type appAction =
  | BoardAction(boardAction)
  | BoxAction(boxAction);

type appState = {
  boardId: string,
  remoteId: option(string),
  title: option(string),
  isDragStart: bool,
  boardAction,
  data: remoteData,
  position: Box.position,
  scale: float,
  movePosition: Box.position,
  eventLog: Event.eventLog,
  minOrder: int,
  maxOrder: int,
  searchQuery: option(string),
};

let minOrder = (l: list(Box.t)) =>
  List.fold_left((acc, v: Box.t) => acc > v.order ? v.order : acc, 0, l);
let maxOrder = (l: list(Box.t)) =>
  List.fold_left((acc, v: Box.t) => acc < v.order ? v.order : acc, 0, l);

let scale = (v, s) => int_of_float(float_of_int(v) /. s);
let mapSelectBox = (appState, f: Box.t => appState) => {
  switch (appState.boardAction) {
  | Select(box) => f(box)
  | _ => appState
  };
};

let newState = state => {
  state->mapSelectBox(box =>
    {
      ...state,
      data:
        RemoteData.map(
          boxes => boxes->Belt.Map.set(box.id, box),
          state.data,
        ),
    }
  );
};

let boardReduce = (state, boardAction) => {
  switch (boardAction) {
  | Init(boardId) => {...state, boardId}
  | EditTitle(title) => {...state, title: title == "" ? None : Some(title)}
  | UpdateBoardId(remoteId) => {...state, remoteId: Some(remoteId)}
  | UpdateBoxId(oldId, newId) =>
    state->mapSelectBox(box =>
      {
        ...state,
        boardAction: Select({...box, id: newId, isRemote: true}),
        data:
          RemoteData.map(
            boxes =>
              boxes
              ->BoxList.set({...box, id: newId, isRemote: true})
              ->BoxList.remove(oldId),
            state.data,
          ),
      }
    )
  | LoadingBoard => {...state, data: RemoteData.Loading(BoxList.empty)}
  | LoadedBoard({remoteId, boardId, title, boxes, scale, position}) =>
    let minOrder = boxes->minOrder;
    let maxOrder = boxes->maxOrder;
    {
      ...state,
      boardId,
      remoteId,
      title,
      data:
        RemoteData.map(
          base => BoxList.merge(base, boxes->BoxList.fromList),
          state.data,
        ),
      eventLog: boxes->Event.init,
      position,
      scale,
      minOrder,
      maxOrder,
    };
  | Error(err) => {...state, data: RemoteData.Failure(err)}
  | Cursor => {...newState(state), boardAction}
  | StartMoveBoard(position) => {
      ...state,
      boardAction: MoveBoard(position),
      movePosition: position,
    }
  | MoveBoard(position) =>
    let (x, y) = position;
    let (currentX, currentY) = state.position;
    let (moveX, moveY) = state.movePosition;
    {
      ...state,
      position: (currentX + (x - moveX), currentY + (y - moveY)),
      movePosition: position,
      boardAction,
    };
  | StopMoveBoard => {...state, boardAction}
  | Adding(_) => {...newState(state), boardAction}
  | Add(box) =>
    switch (state.boardAction) {
    | Adding(_) =>
      let (x, y) = box.position;
      let (boardX, boardY) = state.position;
      {
        ...state,
        boardAction:
          Select({
            ...box,
            position: (
              scale(x - boardX, state.scale),
              scale(y - boardY, state.scale),
            ),
          }),
        data:
          RemoteData.andThen(
            boxes => boxes->BoxList.set(box)->RemoteData.succeed,
            state.data,
          ),
        eventLog: Event.add(state.eventLog, box),
      };
    | _ => state
    }
  | Select(box) => {
      ...state,
      data:
        RemoteData.map(
          boxes =>
            boxes->BoxList.update(box.id, _ => {...box, status: Box.None}),
          state.data,
        ),
      boardAction,
    }
  | UnSelect(box) =>
    let kind =
      switch (box.kind) {
      | Pen(positions, draw, drawList, _) =>
        Box.Pen(positions, draw, drawList, true)
      | _ => box.kind
      };
    switch (state.boardAction) {
    | Adding(_) => state
    | Select(_) => {
        ...state,
        data:
          RemoteData.map(
            boxes =>
              boxes->BoxList.update(box.id, _ =>
                {...box, kind, status: Box.None}
              ),
            state.data,
          ),
        eventLog:
          Event.update(state.eventLog, {...box, kind, status: Box.None}),
        boardAction: Cursor,
      }
    | _ => state
    };
  | ZoomIn => {...state, scale: state.scale -. 0.01}
  | ZoomOut => {...state, scale: state.scale +. 0.01}
  | Undo =>
    let current = state.eventLog->Event.undo;
    {
      ...state,
      eventLog: {
        ...state.eventLog,
        events: current,
      },
      boardAction: Cursor,
      data:
        RemoteData.map(_ => current.current->BoxList.fromList, state.data),
    };
  | Redo =>
    let current = state.eventLog->Event.redo;
    {
      ...state,
      eventLog: {
        ...state.eventLog,
        events: current,
      },
      boardAction: Cursor,
      data:
        RemoteData.map(_ => current.current->BoxList.fromList, state.data),
    };
  | Search(searchQuery) => {...state, searchQuery}
  };
};

let boxReduce = (state, action) => {
  switch (action) {
  | Loading(loading) =>
    state->mapSelectBox(box =>
      {...state, boardAction: Select({...box, loading})}
    )
  | EditUrl(url) =>
    state->mapSelectBox(box =>
      switch (box.kind) {
      | Box.Web(_, page) => {
          ...state,
          boardAction: Select({...box, kind: Web(url, page)}),
        }
      | _ => state
      }
    )
  | LoadUrlComplete(Belt.Result.Ok(page)) =>
    state->mapSelectBox(box =>
      switch (box.kind) {
      | Box.Web(url, _) => {
          ...state,
          boardAction:
            Select({...box, loading: false, kind: Web(url, Some(page))}),
        }
      | _ => state
      }
    )
  | LoadUrlComplete(Belt.Result.Error(_)) =>
    state->mapSelectBox(box =>
      switch (box.kind) {
      | Box.Web(_, _) => {
          ...state,
          boardAction:
            Select({...box, loading: false, kind: Error("Error")}),
        }
      | _ => state
      }
    )
  | EditMarkdown(text) =>
    state->mapSelectBox(box =>
      switch (box.kind) {
      | Box.Markdown(_, color, fontSize) => {
          ...state,
          boardAction:
            Select({...box, kind: Markdown(text, color, fontSize)}),
        }
      | _ => state
      }
    )
  | StartMove(position, status) =>
    switch (state.boardAction) {
    | Select(b) when !b.pinned => {
        ...state,
        boardAction: Select({...b, movePosition: position, status}),
      }
    | _ => state
    }
  | Move(position) =>
    switch (state.boardAction) {
    | Select(b)
        when
          switch (b.kind) {
          | Pen(_, _, _, isEdited) => !isEdited && b.status == Move
          | _ => false
          } =>
      switch (b.kind) {
      | Pen(points, draw, drawList, _) =>
        let (x, y) = position;
        let (currentX, currentY) = b.position;
        let (boardX, boardY) = state.position;
        {
          ...state,
          boardAction:
            Select({
              ...b,
              movePosition: position,
              kind:
                Box.addPenPoints(
                  points,
                  (
                    scale(x, state.scale)
                    - currentX
                    - scale(boardX, state.scale),
                    scale(y, state.scale)
                    - currentY
                    - scale(boardY, state.scale),
                  ),
                  draw,
                  drawList,
                ),
            }),
        };
      | _ => state
      }
    | Select(b) when b.pinned => state
    | Select(b) when b.status == Move =>
      let (x, y) = b.position;
      let (fromX, fromY) = b.movePosition;
      let (toX, toY) = position;
      let x = x + scale(toX - fromX, state.scale);
      let y = y + scale(toY - fromY, state.scale);

      {
        ...state,
        boardAction:
          Select({...b, position: (x, y), movePosition: position}),
      };
    | Select(b) when b.status == ResizeTopLeft =>
      let (x, y) = b.position;
      let (fromX, fromY) = b.movePosition;
      let (toX, toY) = position;
      let (width, height) = b.size;
      let x = x + scale(toX - fromX, state.scale);
      let y = y + scale(toY - fromY, state.scale);
      {
        ...state,
        boardAction:
          Select({
            ...b,
            position: (x, y),
            movePosition: position,
            size: (
              width + scale(fromX - toX, state.scale),
              height + scale(fromY - toY, state.scale),
            ),
          }),
      };
    | Select(b) when b.status == ResizeTopRight =>
      let (x, y) = b.position;
      let (fromX, fromY) = b.movePosition;
      let (toX, toY) = position;
      let (width, height) = b.size;
      let y = y + scale(toY - fromY, state.scale);
      {
        ...state,
        boardAction:
          Select({
            ...b,
            position: (x, y),
            movePosition: position,
            size: (
              width + scale(toX - fromX, state.scale),
              height - scale(toY - fromY, state.scale),
            ),
          }),
      };
    | Select(b) when b.status == ResizeBottomLeft =>
      let (x, y) = b.position;
      let (fromX, fromY) = b.movePosition;
      let (toX, toY) = position;
      let (width, height) = b.size;
      let x = scale(x - (fromX - toX), state.scale);
      {
        ...state,
        boardAction:
          Select({
            ...b,
            position: (x, y),
            movePosition: position,
            size: (
              width + scale(fromX - toX, state.scale),
              height + scale(toY - fromY, state.scale),
            ),
          }),
      };
    | Select(b) when b.status == ResizeBottomRight =>
      let (fromX, fromY) = b.movePosition;
      let (toX, toY) = position;
      let (width, height) = b.size;
      {
        ...state,
        boardAction:
          Select({
            ...b,
            movePosition: position,
            size: (
              width + scale(toX - fromX, state.scale),
              height + scale(toY - fromY, state.scale),
            ),
          }),
      };
    | _ => state
    }
  | StopMove =>
    switch (state.boardAction) {
    | Select(b)
        when
          switch (b.kind) {
          | Pen(_, _, _, isEdited) => !isEdited
          | _ => false
          } =>
      switch (b.kind) {
      | Pen(_, draw, drawList, _) => {
          ...state,
          boardAction:
            Select({
              ...b,
              status: Box.None,
              movePosition: (0, 0),
              kind:
                Pen(
                  [],
                  {id: None, d: "", color: draw.color},
                  [draw, ...drawList],
                  false,
                ),
            }),
        }
      | _ => state
      }
    | Select(b)
        when
          !b.pinned
          && (
            b.status == Move
            || b.status == ResizeBottomLeft
            || b.status == ResizeBottomRight
            || b.status == ResizeTopRight
            || b.status == ResizeTopLeft
          ) =>
      let newBox = {...b, status: Box.None, movePosition: (0, 0)};
      {
        ...state,
        boardAction: Select(newBox),
        eventLog: Event.update(state.eventLog, newBox),
      };
    | MoveBoard(_) => {...state, boardAction: Cursor}
    | _ => state
    }
  | Remove(box) => {
      ...state,
      boardAction: Cursor,
      data:
        RemoteData.map(boxes => boxes->BoxList.remove(box.id), state.data),
      eventLog: Event.delete(state.eventLog, box),
    }
  | StartResize(position, status) =>
    switch (state.boardAction) {
    | Select(b) when !b.pinned => {
        ...state,
        boardAction: Select({...b, status, movePosition: position}),
        eventLog: Event.update(state.eventLog, b),
      }
    | _ => state
    }
  | BringToFront =>
    state->mapSelectBox(box => {
      let frontBox = {...box, order: state.maxOrder + 1};
      let data =
        RemoteData.map(
          boxes => boxes->BoxList.update(frontBox.id, _ => frontBox),
          state.data,
        );

      {
        ...state,
        boardAction: Select(frontBox),
        data,
        maxOrder: state.maxOrder + 1,
      };
    })
  | SendBackward =>
    state->mapSelectBox(box => {
      let backBox = {...box, order: state.minOrder - 1};
      let data =
        RemoteData.map(
          boxes => boxes->BoxList.update(backBox.id, _ => backBox),
          state.data,
        );
      {
        ...state,
        boardAction: Select(backBox),
        data,
        minOrder: state.minOrder - 1,
      };
    })
  | LoadFile(kind, url) =>
    state->mapSelectBox(box => {
      switch (kind) {
      | Image(_) => {
          ...state,
          boardAction: Select({...box, kind: Image(Some(url))}),
        }
      | _ => {
          ...state,
          boardAction:
            Select({
              ...box,
              kind: Markdown(url, Color.transparent, FontSize.default),
            }),
        }
      }
    })
  | ColorChanged(color) =>
    state->mapSelectBox(box => {
      {
        ...state,
        boardAction:
          Select({
            ...box,
            kind:
              switch (box.kind) {
              | Pen(position, draw, drawList, _) =>
                Pen(position, {...draw, color}, drawList, false)
              | Arrow(_, arrowType, angle, strokeWidth) =>
                Arrow(color, arrowType, angle, strokeWidth)
              | Markdown(text, _, fontSize) =>
                Markdown(text, color, fontSize)
              | _ => box.kind
              },
          }),
      }
    })
  | FontSizeChanged(fontSize) =>
    state->mapSelectBox(box => {
      switch (box.kind) {
      | Markdown(text, color, _) => {
          ...state,
          boardAction:
            Select({...box, kind: Markdown(text, color, fontSize)}),
        }
      | _ => state
      }
    })
  | ArrowTypeChanged(arrowType) =>
    state->mapSelectBox(box => {
      switch (box.kind) {
      | Arrow(color, _, angle, strokeWidth) => {
          ...state,
          boardAction:
            Select({
              ...box,
              kind: Arrow(color, arrowType, angle, strokeWidth),
            }),
        }
      | _ => state
      }
    })
  | StrokeWidthChanged(strokeWidth) =>
    state->mapSelectBox(box => {
      switch (box.kind) {
      | Arrow(color, arrowType, angle, _) => {
          ...state,
          boardAction:
            Select({
              ...box,
              kind: Arrow(color, arrowType, angle, strokeWidth),
            }),
        }
      | _ => state
      }
    })
  | Pinned(pin) =>
    state->mapSelectBox(box => {
      {...state, boardAction: Select({...box, pinned: pin})}
    })
  | Copy(box) =>
    let cloneBox = {
      ...Box.newBox(~kind=box.kind, ()),
      position: (100, 100),
      size: box.size,
    };
    {
      ...state,
      data:
        RemoteData.map(boxes => boxes->BoxList.set(cloneBox), state.data),
      boardAction: Select(cloneBox),
    };
  | _ => state
  };
};

let appReducer = (state, action) => {
  switch (action) {
  | BoardAction(action) => boardReduce(state, action)
  | BoxAction(action) => boxReduce(state, action)
  };
};

let saveBoard = (id, board) => {
  Dom.Storage.setItem(
    id,
    Js.Json.stringify(Box.boxData_encode(board)),
    Dom.Storage.localStorage,
  );
};

let thunk = (store, next, action) => {
  let state = Reductive.Store.getState(store);
  let saveState = s => {
    switch (s.data) {
    | RemoteData.Success(boxes) =>
      switch (s.boardAction) {
      | Select(box) =>
        saveBoard(
          s.boardId,
          {
            id: s.boardId,
            title: s.title,
            boxes: boxes->BoxList.update(box.id, _ => box)->BoxList.toList,
            position: s.position,
            scale: s.scale,
          },
        )
      | _ =>
        saveBoard(
          s.boardId,
          {
            id: s.boardId,
            title: s.title,
            boxes: boxes->BoxList.toList,
            position: s.position,
            scale: s.scale,
          },
        )
      };
      next(action);
    | _ => next(action)
    };
    next(action);
  };
  switch (action) {
  | BoardAction(boardAction) =>
    switch (boardAction) {
    | Add(_) => saveState(state)
    | UpdateBoxId(_) => saveState(boardReduce(state, boardAction))
    | UnSelect(_) => saveState(state)
    | StopMoveBoard => saveState(state)
    | _ => next(action)
    }
  | BoxAction(boxAction) =>
    switch (boxAction) {
    | Remove(_) => saveState(boxReduce(state, boxAction))
    | LoadFileEvent(kind, file) =>
      let loadFile: ('a, 'b, 'c) => unit =
        switch (kind) {
        | Image(_) =>
          %bs.raw
          {|
          (file, next, action) => {
              const reader = new FileReader();
              reader.onload = (e) => {
                  next(action(e.target.result));
              }
              reader.readAsDataURL(file);
          }|}
        | _ =>
          %bs.raw
          {|
          (file, next, action) => {
              const reader = new FileReader();
              reader.onload = (e) => {
                  next(action(e.target.result));
              }
              reader.readAsText(file);
          }|}
        };
      loadFile(file, next, url => BoxAction(LoadFile(kind, url)));
    | LoadUrl(url) =>
      let encodedUrl = encodeURIComponent(url);
      Js.Promise.(
        Fetch.fetch({j|$(apiUrl)/crawl/$(encodedUrl)|j})
        |> then_(Fetch.Response.json)
        |> then_(json => {
             let page =
               switch (Box.page_decode(json)) {
               | Ok(page) => Ok(page)
               | Error(e) => Error(e.message)
               };
             next(BoxAction(LoadUrlComplete(page)));
             Some(page) |> resolve;
           })
        |> catch(_ => {
             next(
               BoxAction(
                 LoadUrlComplete(Belt.Result.Error("OGP NOT FOUND")),
               ),
             );
             None |> resolve;
           })
      )
      |> ignore;
      next(BoxAction(Loading(true)));
    | StopMove => saveState(state)
    | _ => next(action)
    }
  };
};

let appStore =
  Reductive.Store.create(
    ~reducer=appReducer,
    ~preloadedState={
      boardId: "",
      remoteId: None,
      title: None,
      isDragStart: false,
      boardAction: Cursor,
      data: RemoteData.succeed(BoxList.empty),
      position: (0, 0),
      scale: 1.0,
      movePosition: (0, 0),
      eventLog: Event.empty(),
      minOrder: 0,
      maxOrder: 0,
      searchQuery: None,
    },
    ~enhancer=(store, next) => thunk(store) @@ next,
    (),
  );

include ReductiveContext.Make({
  type action = appAction;
  type state = appState;
});
