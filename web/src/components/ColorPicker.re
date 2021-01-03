[%bs.raw {|require('./ColorPicker.scss')|}];

[@react.component]
let make = (~currentColor: Color.t) => {
  <div className="color-picker">
    {React.array(
       Array.of_list(
         List.map(
           color => {
             let isSelected =
               Color.toString(currentColor) == Color.toString(color);
             <ColorItem key={Color.toString(color)} color isSelected />;
           },
           Color.colors,
         ),
       ),
     )}
  </div>;
};
