open AppStore;

[%bs.raw {|require('./BoxSettings.scss')|}];

let scaleUp = (v, scale: float) => int_of_float(float_of_int(v) *. scale);

[@react.component]
let make =
    (
      ~box: Box.t,
      ~position,
      ~scale: float,
      ~onDelete: React.callback(Box.t, unit),
    ) => {
  let dispatch = useDispatch();
  let (width, height) = box.size;
  let (offsetX, offsetY) = position;
  let (x, y) = box.position;
  let (_, screenHeight) = Utils.screenSize();
  let newX =
    x->scaleUp(scale)
    - (130 - int_of_float(float_of_int(width->scaleUp(scale)) /. 2.0))
    + offsetX;
  let newY = y->scaleUp(scale) + height->scaleUp(scale) + 64 + offsetY;
  let newY = newY + 230 > screenHeight ? y->scaleUp(scale) - 64 : newY;
  let fixed = box.pinned ? "fixed" : "";

  let fontSizeChanged = e => {
    dispatch(
      BoxAction(
        FontSizeChanged(int_of_string(ReactEvent.Form.target(e)##value)),
      ),
    );
  };

  let arrowTypeChanged = e => {
    dispatch(
      BoxAction(
        ArrowTypeChanged(
          ArrowType.fromString(ReactEvent.Form.target(e)##value),
        ),
      ),
    );
  };

  <div
    style={ReactDOMRe.Style.make(
      ~width="260px",
      ~transition="top 0.05s linear",
      ~position="fixed",
      ~top={j|$(newY)px|j},
      ~left={j|$(newX)px|j},
      (),
    )}>
    <div className="box-settings">
      <div className="buttons">
        <div
          className="button"
          onClick={e => {
            ReactEvent.Mouse.stopPropagation(e);
            dispatch(BoxAction(BringToFront));
          }}>
          "Front"->React.string
        </div>
        <div
          className="button"
          onClick={e => {
            ReactEvent.Mouse.stopPropagation(e);
            dispatch(BoxAction(SendBackward));
          }}>
          "Back"->React.string
        </div>
        <div
          className={j|button $fixed|j}
          onClick={e => {
            ReactEvent.Mouse.stopPropagation(e);
            dispatch(BoxAction(Copy(box)));
          }}>
          <Icon icon=Icon.faCopy className="fa-lg" />
        </div>
        <div
          className={j|button $fixed|j}
          onClick={e => {
            ReactEvent.Mouse.stopPropagation(e);
            dispatch(BoxAction(Pinned(!box.pinned)));
          }}>
          <Icon icon=Icon.faThumbtack className="fa-lg" />
        </div>
        <div
          className="button"
          onClick={_ => {
            onDelete(box);
            dispatch(BoxAction(Remove(box)));
          }}>
          <Icon icon=Icon.faTrash className="fa-lg" />
        </div>
      </div>
      {switch (box.kind) {
       | Markdown(_, _, fontSize) =>
         <div className="select">
           <select value={j|$(fontSize)|j} onChange=fontSizeChanged>
             {React.array(
                List.map(
                  ((v, t)) =>
                    <option key={j|$v|j} value={j|$v|j}>
                      t->React.string
                    </option>,
                  FontSize.fontSizeList,
                )
                ->Array.of_list,
              )}
           </select>
         </div>
       | Arrow(_, arrowType, _) =>
         <div className="select">
           <select
             value={arrowType->ArrowType.toString}
             key={arrowType->ArrowType.toString}
             onChange=arrowTypeChanged>
             <option value={ArrowType.None->ArrowType.toString}>
               "None"->React.string
             </option>
             <option value={ArrowType.Arrow->ArrowType.toString}>
               {0x2192->Js.String2.fromCharCode->React.string}
             </option>
           </select>
         </div>
       | _ => <div />
       }}
      {switch (box.kind) {
       | Pen(_, draw, _, _) => <ColorPicker currentColor={draw.color} />
       | Square(color) => <ColorPicker currentColor=color />
       | Arrow(color, _, _) => <ColorPicker currentColor=color />
       | Markdown(_, color, _) => <ColorPicker currentColor=color />
       | _ => <div />
       }}
    </div>
  </div>;
};
