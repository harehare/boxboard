open AppStore;

[%bs.raw {|require('./Menu.scss')|}];

let scaleSelector = state => state.scale;
let eventLogSelector = state => state.eventLog;

[@react.component]
let make = (~onDownload: unit => unit) => {
  let dispatch = useDispatch();
  let scale = useSelector(scaleSelector);
  let eventLog = useSelector(eventLogSelector);
  let scale = int_of_float(scale *. 100.0 +. 0.05);

  let undoDisabled =
    if (List.length(eventLog.events.events) > 0) {
      "enabled-button";
    } else {
      "disabled-button";
    };

  let redoDisabled =
    if (List.length(eventLog.events.restoreEvents) > 0) {
      "enabled-button";
    } else {
      "disabled-button";
    };

  <div className="menu">
    <div className="menu-button enabled-button">
      <a href="/" target="_blank" rel="noopener noreferrer">
        <Icon icon=Icon.faPlusCircle className="icon" />
        <span className="tooltip">
          <span className="text"> "New"->React.string </span>
        </span>
      </a>
    </div>
    <div className="menu-button enabled-button" onClick={_ => onDownload()}>
      <Icon icon=Icon.faDownload />
      <span className="tooltip">
        <span className="text"> "Download"->React.string </span>
      </span>
    </div>
    <div
      className={j|menu-button $undoDisabled|j}
      onClick={_ => dispatch(BoardAction(Undo))}>
      <Icon icon=Icon.faUndo />
      <span className="tooltip">
        <span className="text"> "Undo"->React.string </span>
      </span>
    </div>
    <div
      className={j|menu-button $redoDisabled|j}
      onClick={_ => dispatch(BoardAction(Redo))}>
      <Icon icon=Icon.faRedo />
      <span className="tooltip">
        <span className="text"> "Redo"->React.string </span>
      </span>
    </div>
    <div
      className="menu-button enabled-button"
      onClick={_ => dispatch(BoardAction(ZoomIn))}>
      <Icon icon=Icon.faMinus />
      <span className="tooltip">
        <span className="text"> "Zoom-out"->React.string </span>
      </span>
    </div>
    <div className="scale"> {j|$(scale)%|j}->React.string </div>
    <div
      className="menu-button enabled-button"
      onClick={_ => dispatch(BoardAction(ZoomOut))}>
      <Icon icon=Icon.faPlus />
      <span className="tooltip">
        <span className="text"> "Zoom-in"->React.string </span>
      </span>
    </div>
  </div>;
};