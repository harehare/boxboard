[%bs.raw {|require('./Notification.scss')|}];

[@react.component]
let make = (~msg: string) => {
  <div className="notification">
    <div className="msg"> <div> msg->React.string </div> </div>
  </div>;
};
