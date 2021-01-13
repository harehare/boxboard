[%bs.raw {|require('./Map.scss')|}];
[@bs.val] external window: Js.t({..}) = "window";

[@react.component]
let make = (~boxList: list(Box.t), ~scale, ~position, ~visible: bool) => {
  let width = visible ? 1920 : 0;
  let height = visible ? 1080 : 0;
  let (x, y) = position;
  let boxX = 0 - x;
  let boxY = 0 - y;
  let boxWidth =
    (float_of_int(window##parent##screen##width) /. scale)->int_of_float;
  let boxHeight =
    (float_of_int(window##parent##screen##height) /. scale)->int_of_float;

  <div
    className="map"
    style={ReactDOMRe.Style.make(
      ~height=visible ? "160px" : "0px",
      ~margin=visible ? "8px" : "0px",
      ~boxShadow=visible ? " 0 2px 6px rgba(5, 0, 56, 0.2)" : "none",
      (),
    )}>
    {if (visible) {
       <svg
         version="1.1"
         width="270"
         height="150"
         viewBox={j|0 0 $width $height|j}
         xmlns="http://www.w3.org/2000/svg">
         <g transform="scale(0.3)">
           {Array.of_list(
              List.map(
                (box: Box.t) => <g key={box.id}> <MapBox box /> </g>,
                boxList,
              ),
            )
            ->React.array}
           <rect
             x={j|$boxX|j}
             y={j|$boxY|j}
             width={j|$boxWidth|j}
             height={j|$boxHeight|j}
             fill="transparent"
             stroke="#333333"
             strokeWidth="40"
           />
         </g>
       </svg>;
     } else {
       <div />;
     }}
  </div>;
};
