open AppStore;
[@bs.module] external uuidv4: unit => string = "uuid-random";
[%bs.raw {|require('./Board.scss')|}];
[@bs.module] external logo: string = "../assets/logo.svg";

let boardActionSelector = state => state.boardAction;
let dataSelector = state => state.data;
let positionSelector = state => state.position;
let scaleSelector = state => state.scale;
let searchQuerySelector = state => state.searchQuery;
let remoteIdSelector = state => state.remoteId;

[@react.component]
let make = (~id) => {
  let dispatch = useDispatch();
  let boardAction = useSelector(boardActionSelector);
  let data = useSelector(dataSelector);
  let position = useSelector(positionSelector);
  let scale = useSelector(scaleSelector);
  let searchQuery = useSelector(searchQuerySelector);
  let remoteId = useSelector(remoteIdSelector);
  let (width, height) = Utils.screenSize();
  let (x, y) = position;
  let {isAuthenticated} = Auth0.useAuth0();
  let {error: queryError, loading: queryLoading} = Hooks.useQuery(id);
  let {error: mutationError, loading: mutationLoading, mutation} =
    Hooks.useMutation(id);
  let onDelete =
    React.useCallback(box =>
      if (isAuthenticated) {
        mutation(Delete(box));
      }
    );
  let updateBox =
    React.useCallback1(
      box =>
        if (isAuthenticated) {
          mutation(Update(box));
        },
      [|isAuthenticated|],
    );
  let onSaveBoard =
    React.useCallback1(
      title =>
        switch (remoteId, isAuthenticated) {
        | (Some(remoteId), true) =>
          mutation(
            SaveBoard({
              remoteId: Some(remoteId),
              boardId: id,
              title: Some(title),
              position,
              scale,
            }),
          )
        | (None, true) =>
          mutation(
            SaveBoard({
              remoteId: None,
              boardId: id,
              title: Some(title),
              position,
              scale,
            }),
          )
        | _ => ()
        },
      [|isAuthenticated|],
    );
  let onDownload =
    React.useCallback1(
      (boxList: BoxList.t, ()) => {
        let (width, height) = boxList->BoxList.size;
        let downloadSvg = [%bs.raw
          {|
      (width, height) => {
        const svgHtml = document.getElementById("boxboard");
        const svg = document.createElementNS(
          "http://www.w3.org/2000/svg",
          "svg"
        );
        svg.setAttribute("viewBox", `0 0 ${width} ${height}`);
        svg.setAttribute("width", width.toString());
        svg.setAttribute("height", height.toString());
        svg.setAttribute("style", svg.getAttribute("style") ?? "");
        svg.innerHTML = svgHtml.innerHTML;

        const svgUrl = `data:image/svg+xml;utf8,${encodeURIComponent(
          new XMLSerializer().serializeToString(svg)
        )}`;
        const dl = document.createElement("a");
        document.body.appendChild(dl);
        dl.setAttribute("href", svgUrl);
        dl.setAttribute("download", "board.svg");
        dl.click();
    }
|}
        ];
        downloadSvg(width, height);
      },
      [|data|],
    );

  let cursorStyle =
    switch (boardAction) {
    | Adding(_) => ""
    | _ => "selected"
    };
  let penStyle =
    switch (boardAction) {
    | Adding(Pen(_)) => "selected"
    | _ => ""
    };
  let markdownStyle =
    switch (boardAction) {
    | Adding(Markdown(_)) => "selected"
    | _ => ""
    };
  let imageStyle =
    switch (boardAction) {
    | Adding(Image(_)) => "selected"
    | _ => ""
    };
  let webStyle =
    switch (boardAction) {
    | Adding(Web(_)) => "selected"
    | _ => ""
    };
  let arrowStyle =
    switch (boardAction) {
    | Adding(Arrow(_)) => "selected"
    | _ => ""
    };

  let keyDown = (e: ReactEvent.Keyboard.t) => {
    switch (boardAction) {
    | Select(box) =>
      ReactEvent.Keyboard.stopPropagation(e);
      switch (ReactEvent.Keyboard.keyCode(e)) {
      // delete
      | 46 => dispatch(BoxAction(Remove(box)))
      | _ => ()
      };
    | _ => ()
    };
  };

  React.useEffect(() => {
    dispatch(BoardAction(Init(id)));
    Document.addEventListener(Document.window, "keydown", keyDown);
    Some(
      () => Document.removeEventListener(Document.window, "keydown", keyDown),
    );
  });

  let boxList =
    React.useMemo2(
      () =>
        switch (data, searchQuery) {
        | (RemoteData.Success(boxes), None) => boxes->BoxList.toList
        | (RemoteData.Success(boxes), Some(query)) =>
          boxes->BoxList.filter((v: Box.t) =>
            switch (v.kind) {
            | Box.Markdown(text, _, _) =>
              Js.String2.indexOf(
                text->String.lowercase_ascii,
                query->String.lowercase_ascii,
              )
              != (-1)
            | Box.Web(_, Some(page)) =>
              Js.String2.indexOf(
                page.title
                ->Belt.Option.getWithDefault("")
                ->String.lowercase_ascii,
                query->String.lowercase_ascii,
              )
              != (-1)
              || Js.String2.indexOf(
                   page.description
                   ->Belt.Option.getWithDefault("")
                   ->String.lowercase_ascii,
                   query->String.lowercase_ascii,
                 )
              != (-1)
            | _ => true
            }
          )
        | _ => []
        },
      (data, searchQuery),
    );

  <div
    className="board"
    style={ReactDOMRe.Style.make(
      ~cursor=
        switch (boardAction) {
        | MoveBoard(_) => "grabbing"
        | _ => "grab"
        },
      (),
    )}>
    {switch (queryError, mutationError) {
     | (Some(err), None) => <Notification msg={err->Err.toString} />
     | (None, Some(err)) => <Notification msg={err->Err.toString} />
     | _ => <div />
     }}
    <ProgressBar
      show={queryLoading || mutationLoading || RemoteData.isLoading(data)}
    />
    <Header />
    <Title onSaveBoard />
    <Menu
      boxList
      position
      onDownload={onDownload(
        {switch (data) {
         | RemoteData.Success(boxes) => boxes
         | _ => BoxList.empty
         }},
      )}
    />
    {switch (boardAction) {
     | Select(box) => <BoxSettings box onDelete />
     | _ => <div />
     }}
    <div className="toolbar">
      <div
        className={j|$cursorStyle button|j}
        onClick={_ => dispatch(BoardAction(Cursor))}>
        <Icon icon=Icon.faMousePointer className="fa-lg" />
        <span className="tooltip">
          <span className="text"> "Cursor"->React.string </span>
        </span>
      </div>
      <div
        className={j|$penStyle button|j}
        onClick={_ =>
          dispatch(
            BoardAction(
              Adding(
                Box.Pen(
                  [],
                  {id: Some(uuidv4()), d: "", color: Color.black},
                  [],
                  false,
                ),
              ),
            ),
          )
        }>
        <Icon icon=Icon.faPen className="fa-lg" />
        <span className="tooltip">
          <span className="text"> "Pen"->React.string </span>
        </span>
      </div>
      <div
        className={j|$markdownStyle button|j}
        onClick={_ =>
          dispatch(
            BoardAction(
              Adding(Box.Markdown("", Color.yellow, FontSize.default)),
            ),
          )
        }>
        <Icon icon=Icon.faStickyNote className="fa-lg" />
        <span className="tooltip">
          <span className="text"> "Markdown"->React.string </span>
        </span>
      </div>
      <div
        className={j|$imageStyle button|j}
        onClick={_ => dispatch(BoardAction(Adding(Box.Image(None))))}>
        <Icon icon=Icon.faImage className="fa-lg" />
        <span className="tooltip">
          <span className="text"> "Image"->React.string </span>
        </span>
      </div>
      <div
        className={j|$webStyle button|j}
        onClick={_ => dispatch(BoardAction(Adding(Box.Web("", None))))}>
        <Icon icon=Icon.faGlobe className="fa-lg" />
        <span className="tooltip">
          <span className="text"> "Web"->React.string </span>
        </span>
      </div>
      <div
        className={j|$arrowStyle button|j}
        onClick={_ =>
          dispatch(
            BoardAction(
              Adding(Box.Arrow(Color.black, ArrowType.Arrow, 0, 2)),
            ),
          )
        }>
        <Icon icon=Icon.faArrowRight className="fa-lg" />
        <span className="tooltip">
          <span className="text"> "Arrow"->React.string </span>
        </span>
      </div>
    </div>
    {switch (data) {
     | RemoteData.NotAsked => <div />
     | RemoteData.Loading(_) => <div />
     | RemoteData.Failure(_) => <div />
     | RemoteData.Success(boxes) =>
       <ReactDropzone
         accept={`many(["image/*", "text/*", "application/json"])}
         multiple=true
         onDrop={(files, _) =>
           List.iteri(
             (i, file) =>
               switch (
                 Array.to_list(
                   Js.String2.split(file->FileReader.File.type_, "/"),
                 )
               ) {
               | ["image", _] =>
                 let box =
                   Box.newBox(
                     ~kind=Image(None),
                     ~position=(10 * i, 10 * i),
                     (),
                   );
                 dispatch(BoardAction(Adding(Image(None))));
                 dispatch(BoardAction(Add(box)));
                 if (isAuthenticated) {
                   mutation(Add(box));
                 };
                 dispatch(BoxAction(LoadFileEvent(Image(None), file)));
               | _ =>
                 let box =
                   Box.newBox(
                     ~kind=Markdown("", Color.yellow, FontSize.default),
                     ~position=(10 * i, 10 * i),
                     (),
                   );
                 dispatch(
                   BoardAction(
                     Adding(Markdown("", Color.yellow, FontSize.default)),
                   ),
                 );
                 dispatch(BoardAction(Add(box)));
                 if (isAuthenticated) {
                   mutation(Add(box));
                 };
                 dispatch(
                   BoxAction(
                     LoadFileEvent(
                       Markdown("", Color.yellow, FontSize.default),
                       file,
                     ),
                   ),
                 );
               },
             Array.to_list(files),
           )
         }>
         {(
            props => {
              let rootProps = props.getRootProps();
              <div
                onBlur={rootProps.onBlur}
                onDragEnter={rootProps.onDragEnter}
                onDragLeave={rootProps.onDragLeave}
                onDragOver={rootProps.onDragOver}
                onDragStart={rootProps.onDragStart}
                onDrop={rootProps.onDrop}
                onFocus={rootProps.onFocus}
                onKeyDown={rootProps.onKeyDown}
                ref={ReactDOMRe.Ref.callbackDomRef(rootProps.ref)}
                onMouseDown={
                  switch (boardAction) {
                  | Select(box) => (
                      _ => {
                        let currentBox =
                          BoxList.empty
                          ->RemoteData.withDefault(data)
                          ->BoxList.get(box.id)
                          ->Belt.Option.getWithDefault(Box.empty);
                        switch (
                          Box.equals(currentBox, box),
                          isAuthenticated,
                          box.isRemote,
                        ) {
                        | (false, true, true) => mutation(Update(box))
                        | (false, true, false) => mutation(Add(box))
                        | _ => ()
                        };
                        dispatch(BoardAction(UnSelect(box)));
                      }
                    )
                  | Adding(kind) => (
                      (e: ReactEvent.Mouse.t) => {
                        ReactEvent.Mouse.stopPropagation(e);
                        let box =
                          Box.newBox(
                            ~kind,
                            ~position=Utils.getMousePosition(e),
                            (),
                          );
                        dispatch(BoardAction(Add(box)));
                        if (isAuthenticated) {
                          mutation(Add(box));
                        };
                      }
                    )
                  | _ => (
                      (e: ReactEvent.Mouse.t) => {
                        ReactEvent.Mouse.stopPropagation(e);
                        dispatch(
                          BoardAction(
                            StartMoveBoard(Utils.getMousePosition(e)),
                          ),
                        );
                      }
                    )
                  }
                }
                onTouchStart={
                  switch (boardAction) {
                  | Select(box) => (
                      _ => dispatch(BoardAction(UnSelect(box)))
                    )
                  | Adding(kind) => (
                      (e: ReactEvent.Touch.t) => {
                        ReactEvent.Touch.stopPropagation(e);
                        let box =
                          Box.newBox(
                            ~kind,
                            ~position=Utils.getTouchPosition(e),
                            (),
                          );
                        dispatch(BoardAction(Add(box)));
                        if (isAuthenticated) {
                          mutation(Add(box));
                        };
                      }
                    )
                  | _ => (
                      (e: ReactEvent.Touch.t) =>
                        dispatch(
                          BoardAction(
                            StartMoveBoard(Utils.getTouchPosition(e)),
                          ),
                        )
                    )
                  }
                }
                onMouseUp={_ => {
                  switch (boardAction) {
                  | MoveBoard(_) => dispatch(BoxAction(StopMove))
                  | _ => ()
                  }
                }}
                onTouchEnd={_ =>
                  switch (boardAction) {
                  | MoveBoard(_) => dispatch(BoxAction(StopMove))
                  | _ => ()
                  }
                }
                onMouseMove={(e: ReactEvent.Mouse.t) =>
                  switch (boardAction) {
                  | Select(box) =>
                    if (box.status != Box.None) {
                      dispatch(BoxAction(Move(Utils.getMousePosition(e))));
                    }
                  | MoveBoard(_) =>
                    dispatch(
                      BoardAction(MoveBoard(Utils.getMousePosition(e))),
                    )
                  | _ => ()
                  }
                }
                onTouchMove={(e: ReactEvent.Touch.t) =>
                  switch (boardAction) {
                  | Select(_) =>
                    dispatch(BoxAction(Move(Utils.getTouchPosition(e))))
                  | MoveBoard(_) =>
                    dispatch(
                      BoardAction(MoveBoard(Utils.getTouchPosition(e))),
                    )
                  | _ => ()
                  }
                }
                onWheel={(e: ReactEvent.Wheel.t) =>
                  ReactEvent.Wheel.deltaY(e) > 0.0
                    ? dispatch(BoardAction(ZoomIn))
                    : dispatch(BoardAction(ZoomOut))
                }>
                {if (boxes->BoxList.isEmpty) {
                   <div className="full-screen">
                     <div className="logo-area">
                       <img src=logo className="logo" />
                       <div className="name"> "BoxBoard"->React.string </div>
                       <div className="description">
                         "Place the box and create a board"->React.string
                       </div>
                     </div>
                   </div>;
                 } else {
                   <svg
                     id="boxboard"
                     version="1.1"
                     width
                     height
                     viewBox={j|0 0 $width $height|j}
                     xmlns="http://www.w3.org/2000/svg">
                     <g transform={j|translate($x, $y), scale($scale)|j}>
                       {switch (boardAction) {
                        | Select(selectBox) =>
                          React.array(
                            Array.of_list(
                              List.map(
                                (b: Box.t) => {
                                  let (box, isSelected) = (
                                    selectBox.id == b.id ? selectBox : b,
                                    selectBox.id == b.id,
                                  );
                                  <BoxContainer
                                    key={box.id}
                                    box
                                    scale
                                    isSelected
                                    updateBox
                                  />;
                                },
                                boxList,
                              ),
                            ),
                          )
                        | _ =>
                          React.array(
                            Array.of_list(
                              List.map(
                                (box: Box.t) =>
                                  <g key={box.id}>
                                    <BoxContainer
                                      box
                                      scale
                                      isSelected=false
                                      updateBox
                                    />
                                  </g>,
                                boxList,
                              ),
                            ),
                          )
                        }}
                     </g>
                   </svg>;
                 }}
              </div>;
            }
          )}
       </ReactDropzone>
     }}
  </div>;
};
