open AppStore;
[@bs.module] external uuidv4: unit => string = "uuid-random";

[@react.component]
let make =
    (~box: Box.t, ~isSelected, ~updateBox: React.callback(Box.t, unit)) => {
  let dispatch = useDispatch();
  let (width, height) = box.size;
  let (x, y) = box.position;

  let mouseDown = (e: ReactEvent.Mouse.t) =>
    if (isSelected) {
      ReactEvent.Mouse.stopPropagation(e);
      dispatch(BoxAction(StartMove(Utils.getMousePosition(e), Move)));
    };
  let mouseUp = (e: ReactEvent.Mouse.t) =>
    if (isSelected) {
      ReactEvent.Mouse.stopPropagation(e);
      updateBox(box);
      dispatch(BoxAction(StopMove));
    };
  let touchStart = (e: ReactEvent.Touch.t) =>
    if (isSelected) {
      ReactEvent.Touch.stopPropagation(e);
      dispatch(BoxAction(StartMove(Utils.getTouchPosition(e), Move)));
    };
  let touchEnd = (e: ReactEvent.Touch.t) =>
    if (isSelected) {
      ReactEvent.Touch.stopPropagation(e);
      dispatch(BoxAction(StopMove));
    };
  let cornerMouseDown = (e: ReactEvent.Mouse.t, status) =>
    if (isSelected) {
      dispatch(BoxAction(StartMove(Utils.getMousePosition(e), status)));
    };
  let cornerTouchStart = (e: ReactEvent.Touch.t, status) =>
    if (isSelected) {
      dispatch(BoxAction(StartMove(Utils.getTouchPosition(e), status)));
    };
  let editMarkdown = (e: ReactEvent.Form.t) =>
    dispatch(BoxAction(EditMarkdown(ReactEvent.Form.target(e)##value)));
  let boxBorderColor = isSelected ? "#48A9DD" : "transparent";
  let fillColor =
    switch (box.kind) {
    | Square(color) => Color.toString(color)
    | Markdown(_, color, _) => Color.toString(color)
    | _ => "transparent"
    };
  let svgWidth = width + 20 < 260 ? 260 : width + 20;
  let svgHeight = height + 180;
  let foreignObjectWidth = width - 16;
  let foreignObjectHeight = height - 16;

  let onPickImage =
    React.useCallback(e =>
      dispatch(
        BoxAction(
          LoadFileEvent(Image(None), ReactEvent.Form.target(e)##files[0]),
        ),
      )
    );

  let onUrlEdit =
    React.useCallback(e =>
      dispatch(BoxAction(EditUrl(ReactEvent.Form.target(e)##value)))
    );

  let onLoadClick =
    React.useCallback((url: string, _: ReactEvent.Mouse.t) =>
      dispatch(BoxAction(LoadUrl(url)))
    );

  <g transform={j|translate($(x), $(y))|j}>
    <svg
      className="box"
      viewBox={j|0 0 $svgWidth $svgHeight|j}
      width={string_of_int(svgWidth)}
      height={string_of_int(svgHeight)}
      onMouseDown={e => mouseDown(e)}
      onMouseUp={e => mouseUp(e)}
      onTouchStart={e => touchStart(e)}
      onTouchEnd={e => touchEnd(e)}
      onClick={_ => dispatch(AppStore.BoardAction(AppStore.Select(box)))}>
      <rect
        x="5"
        y="5"
        width={string_of_int(width)}
        height={string_of_int(height)}
        fill=fillColor
        strokeWidth="1"
        onMouseMove={e =>
          if (isSelected && box.status != Box.None) {
            dispatch(BoxAction(Move(Utils.getMousePosition(e))));
          }
        }
        stroke=boxBorderColor
      />
      {isSelected && !box.pinned
         ? <g>
             <rect
               x="1"
               y="1"
               width="10"
               height="10"
               fill="#48a9dd"
               rx="30"
               ry="30"
               style={ReactDOMRe.Style.make(~cursor="nwse-resize", ())}
               onMouseDown={(e: ReactEvent.Mouse.t) => {
                 ReactEvent.Mouse.stopPropagation(e);
                 cornerMouseDown(e, ResizeTopLeft);
               }}
               onTouchStart={(e: ReactEvent.Touch.t) => {
                 ReactEvent.Touch.stopPropagation(e);
                 cornerTouchStart(e, ResizeTopLeft);
               }}
             />
             <rect
               x={string_of_int(width)}
               y="1"
               width="10"
               height="10"
               fill="#48a9dd"
               rx="30"
               ry="30"
               style={ReactDOMRe.Style.make(~cursor="nesw-resize", ())}
               onMouseDown={(e: ReactEvent.Mouse.t) => {
                 ReactEvent.Mouse.stopPropagation(e);
                 cornerMouseDown(e, ResizeTopRight);
               }}
               onTouchStart={(e: ReactEvent.Touch.t) => {
                 ReactEvent.Touch.stopPropagation(e);
                 cornerTouchStart(e, ResizeTopRight);
               }}
             />
             <rect
               x="1"
               y={string_of_int(height)}
               width="10"
               height="10"
               fill="#48a9dd"
               rx="30"
               ry="30"
               style={ReactDOMRe.Style.make(~cursor="nesw-resize", ())}
               onMouseDown={(e: ReactEvent.Mouse.t) => {
                 ReactEvent.Mouse.stopPropagation(e);
                 cornerMouseDown(e, ResizeBottomLeft);
               }}
               onTouchStart={(e: ReactEvent.Touch.t) => {
                 ReactEvent.Touch.stopPropagation(e);
                 cornerTouchStart(e, ResizeBottomLeft);
               }}
             />
             <rect
               x={string_of_int(width)}
               y={string_of_int(height)}
               width="10"
               height="10"
               fill="#48a9dd"
               rx="30"
               ry="30"
               style={ReactDOMRe.Style.make(~cursor="nwse-resize", ())}
               onMouseDown={(e: ReactEvent.Mouse.t) => {
                 ReactEvent.Mouse.stopPropagation(e);
                 cornerMouseDown(e, ResizeBottomRight);
               }}
               onTouchStart={(e: ReactEvent.Touch.t) => {
                 ReactEvent.Touch.stopPropagation(e);
                 cornerTouchStart(e, ResizeBottomRight);
               }}
             />
           </g>
         : <g />}
      {switch (box.kind) {
       | Square(_) => <g />
       | Pen(_, draw, drawList, _) => <Pen width height draw drawList />
       | Markdown(text, _, fontSize) =>
         <Markdown
           text
           size=(foreignObjectWidth, foreignObjectHeight)
           fontSize
           isEdit=isSelected
           onChange=editMarkdown
         />
       | Image(Some(imageData)) =>
         <Image
           image=imageData
           size=(foreignObjectWidth, foreignObjectHeight)
           isEdit=isSelected
           onChange=onPickImage
         />
       | Image(None) =>
         <Image
           image=""
           size=(foreignObjectWidth, foreignObjectHeight)
           isEdit=isSelected
           onChange=onPickImage
         />
       | Web(url, page) =>
         <Web
           url
           page
           size=(foreignObjectWidth, foreignObjectHeight)
           isEdit=isSelected
           isLoading={box.loading}
           onUrlChange=onUrlEdit
           onLoadClick={onLoadClick(url)}
         />
       | Arrow(color, arrowType, angle) =>
         <Arrow
           size=(foreignObjectWidth + 16, foreignObjectHeight)
           color
           arrowType
           angle
         />
       | Error(msg) =>
         <Error msg size=(foreignObjectWidth, foreignObjectHeight) />
       }}
    </svg>
  </g>;
};
