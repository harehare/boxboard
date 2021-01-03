[%bs.raw {|require('./ProgressBar.scss')|}];

[@react.component]
let make = (~show: bool) =>
  if (show) {
    <div className="progress"> <div className="indeterminate" /> </div>;
  } else {
    <div style={ReactDOMRe.Style.make(~height="4px", ~width="100%", ())} />;
  };
