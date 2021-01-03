open AppStore;

[%bs.raw {|require('./ColorItem.scss')|}];

[@react.component]
let make = (~color: Color.t, ~isSelected: bool) => {
  let dispatch = useDispatch();
  let selected = isSelected ? "selected-circle" : "";
  <div
    key={Color.toString(color)}
    className={j|color-circle $selected|j}
    onClick={e => {
      ReactEvent.Mouse.stopPropagation(e);
      dispatch(BoxAction(ColorChanged(color)));
    }}
    style={ReactDOMRe.Style.make(~backgroundColor=Color.toString(color), ())}
  />;
};
