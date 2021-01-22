open AppStore;
[%bs.raw {|require('./Title.scss')|}];

let titleSelector = state => state.title;

[@react.component]
let make = (~onSaveBoard: React.callback(string, unit)) => {
  let dispatch = useDispatch();
  let value = useSelector(titleSelector);
  <div className="title">
    <input
      type_="text"
      value={value->Belt.Option.getWithDefault("")}
      className="input-title"
      placeholder="Enter a title"
      onInput={e => {
        let value = ReactEvent.Form.target(e)##value;
        dispatch(BoardAction(EditTitle(value)));
      }}
      onBlur={_ => onSaveBoard(value->Belt.Option.getWithDefault(""))}
      onFocus={e => ReactEvent.Focus.target(e)##select()}
    />
  </div>;
};
