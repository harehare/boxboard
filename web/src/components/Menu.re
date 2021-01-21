open AppStore;

[%bs.raw {|require('./Menu.scss')|}];

let scaleSelector = state => state.scale;
let eventLogSelector = state => state.eventLog;
let searchQuerySelector = state => state.searchQuery;

[@react.component]
let make = (~boxList: list(Box.t), ~position, ~onDownload: unit => unit) => {
  let dispatch = useDispatch();
  let scale = useSelector(scaleSelector);
  let eventLog = useSelector(eventLogSelector);
  let searchQuery = useSelector(searchQuerySelector);
  let scalePer = int_of_float(scale *. 100.0 +. 0.05);
  let (isSearch, onSearch) = React.useState(_ => false);
  let (visibleMap, onVisibleMap) = React.useState(_ => false);
  let inputRef = React.useRef(Js.Nullable.null);

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

  let searchStyle = if (isSearch) {"select-button"} else {""};

  let mapStyle = if (visibleMap) {"select-button"} else {""};

  React.useEffect1(
    () => {
      (
        switch (Js.Nullable.toOption(inputRef.current)) {
        | Some(element) =>
          let elementObj = ReactDOMRe.domElementToObj(element);
          elementObj##focus() |> ignore;
          ();
        | None => ()
        }
      )
      |> ignore;
      None;
    },
    [|isSearch|],
  );

  <div className="menu">
    <Map boxList position scale visible=visibleMap />
    <div className="menu-button enabled-button">
      <a
        href="/"
        target="_blank"
        rel="noopener noreferrer"
        onClick={_ => {
          Dom.Storage.setItem(
            Constants.sessionPageId,
            "",
            Dom.Storage.sessionStorage,
          )
        }}>
        <Icon icon=Icon.faPlusCircle className="icon" />
        <span className="tooltip">
          <span className="text"> "New"->React.string </span>
        </span>
      </a>
    </div>
    <div
      className={j|menu-button enabled-button $searchStyle|j}
      onClick={e => {
        ReactEvent.Mouse.stopPropagation(e);
        onSearch(_ => true);
      }}>
      <div className="search">
        <Icon icon=Icon.faSearch className="icon" />
        <input
          type_="text"
          style={ReactDOMRe.Style.make(
            ~width=isSearch ? "160px" : "0px",
            ~marginLeft=isSearch ? "8px" : "0px",
            ~borderBottom=isSearch ? "1px solid var(--border-color)" : "none",
            (),
          )}
          className="input"
          ref={ReactDOMRe.Ref.domRef(inputRef)}
          placeholder="Search"
          value={Belt.Option.getWithDefault(searchQuery, "")}
          onInput={e => {
            let value = ReactEvent.Form.target(e)##value;
            dispatch(
              BoardAction(Search(value == "" ? None : Some(value))),
            );
          }}
          onBlur={_ => {onSearch(_ => false)}}
        />
      </div>
      <span className="tooltip">
        <span className="text"> "Search"->React.string </span>
      </span>
    </div>
    <div
      className={j|menu-button enabled-button $mapStyle|j}
      onClick={_ => onVisibleMap(p => !p)}>
      <Icon icon=Icon.faMap />
      <span className="tooltip">
        <span className="text"> "Map"->React.string </span>
      </span>
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
    <div className="scale"> {j|$(scalePer)%|j}->React.string </div>
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
