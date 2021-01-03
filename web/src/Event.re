[@bs.module] external uuidv4: unit => string = "uuid-random";

type id = string;

type event =
  | Add(id, Box.t)
  | Update(id, Box.t)
  | Delete(id, Box.t);

type events = {
  events: list(event),
  restoreEvents: list(event),
  current: list(Box.t),
};

type eventLog = {
  events,
  base: list(Box.t),
};

let init = boxes => {
  events: {
    events: [],
    restoreEvents: [],
    current: boxes,
  },
  base: boxes,
};
let empty = () => {
  events: {
    events: [],
    restoreEvents: [],
    current: [],
  },
  base: [],
};

let applyEvent = (current: list(Box.t), event: event) => {
  switch (event) {
  | Add(_, box) => [box, ...current]
  | Update(_, box) =>
    List.map((b: Box.t) => b.id === box.id ? box : b, current)
  | Delete(_, box) => List.filter((b: Box.t) => b.id != box.id, current)
  };
};

let rec apply = (base: list(Box.t), events: list(event)): events =>
  switch (events) {
  | [head, ...rest] =>
    let currentLog = applyEvent(base, head);
    apply(currentLog, rest);
  | [] => {current: base, events, restoreEvents: []}
  };

let undo = (log: eventLog): events => {
  switch (log.events.events) {
  | [head, ...rest] => {
      ...apply(log.base, rest),
      events: rest,
      restoreEvents: [head, ...log.events.restoreEvents],
    }
  | [] => log.events
  };
};

let redo = (log: eventLog): events => {
  switch (log.events.restoreEvents) {
  | [head, ...rest] => {
      ...apply(log.base, [head, ...log.events.events]),
      events: log.events.events,
      restoreEvents: rest,
    }
  | [] => log.events
  };
};

let updateEventLog = (eventLog: eventLog, event): eventLog => {
  events: {
    events: [event, ...eventLog.events.events],
    restoreEvents: [],
    current: eventLog.events.current,
  },
  base: eventLog.base,
};

let add = (eventLog: eventLog, box): eventLog => {
  updateEventLog(eventLog, Add(uuidv4(), box));
};

let update = (eventLog: eventLog, box): eventLog => {
  updateEventLog(eventLog, Update(uuidv4(), box));
};

let delete = (eventLog: eventLog, box): eventLog => {
  updateEventLog(eventLog, Delete(uuidv4(), box));
};
