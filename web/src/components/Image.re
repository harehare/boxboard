open Box;

[%bs.raw {|require('./Image.scss')|}];

[@react.component]
let make =
    (
      ~image: string,
      ~size: position,
      ~isEdit: bool,
      ~onChange: ReactEvent.Form.t => unit,
    ) => {
  let (width, height) = size;
  let imageHeight = height - 16;

  if (isEdit) {
    <foreignObject x="10" y="10" width={j|$width|j} height={j|$height|j}>
      <div style={ReactDOMRe.Style.make(~position="relative", ())}>
        <label className="file-icon">
          <input type_="file" className="input-file" multiple=false onChange />
          <i className="fas icon fa-upload" />
        </label>
        <img
          src=image
          draggable=false
          style={ReactDOMRe.Style.make(
            ~width={j|$(width)px|j},
            ~height={j|$(imageHeight)px|j},
            ~objectFit="contain",
            (),
          )}
        />
      </div>
    </foreignObject>;
  } else {
    <foreignObject x="10" y="10" width={j|$width|j} height={j|$height|j}>
      <img
        src=image
        draggable=false
        style={ReactDOMRe.Style.make(
          ~width={j|$(width)px|j},
          ~height={j|$(height)px|j},
          ~objectFit="contain",
          (),
        )}
      />
    </foreignObject>;
  };
};
