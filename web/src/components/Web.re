open Box;

[%bs.raw {|require('./Web.scss')|}];

[@react.component]
let make =
    (
      ~url: string,
      ~page: option(page),
      ~size: position,
      ~isLoading: bool,
      ~isEdit: bool,
      ~onUrlChange: ReactEvent.Form.t => unit,
      ~onLoadClick: ReactEvent.Mouse.t => unit,
    ) => {
  let (width, height) = size;
  let handleFocus = (e: ReactEvent.Focus.t) =>
    e->ReactEvent.Focus.target##select();

  switch (page, isLoading, isEdit) {
  | (Some(page), false, true) =>
    <foreignObject x="10" y="10" width={j|$width|j} height={j|$height|j}>
      <div className="web">
        <div className="input-area">
          <input
            type_="text"
            className="input-url"
            onChange=onUrlChange
            onFocus=handleFocus
            value=url
            placeholder="URL"
          />
          <button className="load-button" onClick=onLoadClick>
            "Load"->React.string
          </button>
        </div>
        <img
          className="web-image"
          src={Belt.Option.getWithDefault(page.imageUrl, "")}
          draggable=false
        />
        <div className="web-title">
          {React.string(Belt.Option.getWithDefault(page.title, ""))}
        </div>
        <div className="web-description">
          {React.string(Belt.Option.getWithDefault(page.description, ""))}
        </div>
      </div>
    </foreignObject>
  | (Some(page), false, false) =>
    <foreignObject x="10" y="10" width={j|$width|j} height={j|$height|j}>
      <div className="web">
        <img
          className="web-image"
          src={Belt.Option.getWithDefault(page.imageUrl, "")}
          draggable=false
        />
        <a href=url target="_blank" rel="noopener">
          <div className="web-title">
            {Belt.Option.getWithDefault(page.title, "")->React.string}
          </div>
        </a>
        <div className="web-description">
          {Belt.Option.getWithDefault(page.description, "")->React.string}
        </div>
      </div>
    </foreignObject>
  | (_, true, _) =>
    <foreignObject x="10" y="10" width={j|$width|j} height={j|$height|j}>
      <div className="web">
        <div className="shimmer web-image" draggable=false />
        <div className="shimmer web-title" />
        <div className="shimmer web-description" />
      </div>
    </foreignObject>
  | (None, _, true) =>
    <foreignObject x="10" y="10" width={j|$width|j} height={j|$height|j}>
      <div className="web">
        <div className="input-area">
          <input
            type_="text"
            className="input-url"
            onChange=onUrlChange
            value=url
            placeholder="Enter Url"
          />
          <button className="load-button" onClick=onLoadClick>
            "Load"->React.string
          </button>
        </div>
        <div className="empty"> <div> "Empty"->React.string </div> </div>
      </div>
    </foreignObject>
  | _ =>
    <foreignObject x="10" y="10" width={j|$width|j} height={j|$height|j}>
      <div className="web">
        <div className="empty"> <div> "Empty"->React.string </div> </div>
      </div>
    </foreignObject>
  };
};
