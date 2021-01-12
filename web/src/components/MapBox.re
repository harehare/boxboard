[%bs.raw {|require('./MapBox.scss')|}];

[@react.component]
let make = (~box: Box.t) => {
  let (x, y) = box.position;
  let (width, height) = box.size;
  <rect
    fill="rgba(72, 169, 221, 0.4)"
    stroke="#48a9dd"
    strokeWidth="5"
    x={j|$x|j}
    y={j|$y|j}
    width={j|$width|j}
    height={j|$height|j}
  />;
};
