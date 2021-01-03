open Box;

[@bs.module] external uuidv4: unit => string = "uuid-random";

[@react.component]
let make = (~width: int, ~height: int, ~draw: draw, ~drawList: list(draw)) =>
  <svg
    viewBox={j|0 0 $width $height|j} width={j|$width|j} height={j|$height|j}>
    <path
      d={draw.d}
      fill="transparent"
      stroke={Color.toString(draw.color)}
      strokeWidth="2"
    />
    {React.array(
       Array.of_list(
         List.map(
           (d: draw) =>
             <path
               key={Belt.Option.getWithDefault(d.id, uuidv4())}
               d={d.d}
               fill="transparent"
               stroke={Color.toString(d.color)}
               strokeWidth="2"
             />,
           drawList,
         ),
       ),
     )}
  </svg>;
