open AppStore;

[%bs.raw {|require('./Menu.scss')|}];

let scaleSelector = state => state.scale;
let eventLogSelector = state => state.eventLog;
let searchQuerySelector = state => state.searchQuery;

[@react.component]
let make = (~onDownload: unit => unit) => {
  let dispatch = useDispatch();
  let scale = useSelector(scaleSelector);
  let eventLog = useSelector(eventLogSelector);
  let searchQuery = useSelector(searchQuerySelector);
  let scale = int_of_float(scale *. 100.0 +. 0.05);
  let (isSearch, onSearch) = React.useState(_ => false);
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
    <div className="menu-button enabled-button">
      <a href="/" target="_blank" rel="noopener noreferrer">
        <Icon icon=Icon.faPlusCircle className="icon" />
        <span className="tooltip">
          <span className="text"> "New"->React.string </span>
        </span>
      </a>
    </div>
    <div
      className="menu-button enabled-button"
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
