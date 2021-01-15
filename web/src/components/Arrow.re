open Box;

[@react.component]
let make =
    (
      ~size: position,
      ~color: Color.t,
      ~arrowType: ArrowType.t,
      ~angle: int,
      ~strokeWidth: int,
    ) => {
  let (width, height) = size;
  let y = height / 2 + 16;
  <g transform={j|rotate($angle)|j}>
    <marker
      id="arrow"
      viewBox="0 0 10 10"
      refX="5"
      refY="5"
      markerWidth="5"
      markerHeight="5"
      orient="auto-start-reverse">
      <path d="M 0 0 L 10 5 L 0 10 z" />
    </marker>
    <line
      x1="8"
      y1={string_of_int(y)}
      x2={string_of_int(width)}
      y2={string_of_int(y)}
      strokeWidth={j|$strokeWidth|j}
      markerEnd={
        switch (arrowType) {
        | ArrowType.Arrow => "url(#arrow)"
        | ArrowType.None => ""
        }
      }
      fill={Color.toString(color)}
      stroke={Color.toString(color)}
    />
  </g>;
};
