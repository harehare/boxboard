open Box;

[%bs.raw {|require('./Error.scss')|}];

[@react.component]
let make = (~msg: string, ~size: position) => {
  let (width, height) = size;
  <foreignObject x="10" y="10" width={j|$width|j} height={j|$height|j}>
    <div className="error">
      <div className="msg"> <div> msg->React.string </div> </div>
    </div>
  </foreignObject>;
};
