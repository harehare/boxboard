let _useStorage = (storage: Dom_storage2.t, key: string) => {
  let (value, setValue) =
    React.useState(() => Dom.Storage.getItem(key, storage));
  let setItem = (newValue: string) => {
    setValue(_ => Some(newValue));
    Dom.Storage.setItem(key, newValue);
  };
  React.useEffect1(
    () =>
      Some(
        _ =>
          Dom.Storage.setItem(
            key,
            Belt.Option.getWithDefault(value, ""),
            storage,
          ),
      ),
    [|value|],
  );
  (value, setItem);
};

let useLocalStorage = _useStorage(Dom.Storage.localStorage);
let useSessionStorage = _useStorage(Dom.Storage.sessionStorage);

type requestState = {
  error: option(Err.t),
  loading: bool,
};

type mutation =
  | Add(Box.t)
  | Update(Box.t)
  | Delete(Box.t);

type mutationResult = {
  error: option(Err.t),
  loading: bool,
  mutation: mutation => unit,
};

type queryResult = {
  error: option(Err.t),
  loading: bool,
};

let useQuery = (boardId: string): queryResult => {
  let dispatch = AppStore.useDispatch();
  let (state, setState) =
    React.useState(_ => ({error: None, loading: false}: requestState));

  let (localBoardValue, _) = useLocalStorage(boardId);
  let {isAuthenticated, getIdTokenClaims} = Auth0.useAuth0();

  React.useEffect2(
    () => {
      let _ =
        if (isAuthenticated) {
          getIdTokenClaims()
          ->Utils.Promise.then_(token => {
              Dom.Storage.setItem(
                Constants.sessionToken,
                token.__raw,
                Dom.Storage.sessionStorage,
              )
              |> ignore;
              Request.findBoard(boardId)
              ->Utils.Promise.then_(result => {
                  switch (result) {
                  | Ok(boxes) =>
                    dispatch(
                      BoardAction(LoadedBoard(boardId, boxes, true)),
                    );
                    setState(_ => {error: None, loading: false});
                    Js.Promise.resolve();
                  | Error(err) =>
                    setState(_ => {error: Some(err), loading: false});
                    Js.Promise.resolve();
                  }
                })
              ->Utils.Promise.ignore;
              Js.Promise.resolve();
            });
        } else {
          switch (localBoardValue) {
          | None => dispatch(BoardAction(LoadedBoard(boardId, [], false)))
          | Some(j) =>
            let result = Js.Json.parseExn(j) |> Box.boxData_decode;

            if (Belt.Result.isOk(result)) {
              let boxData = Belt.Result.getExn(result);
              dispatch(
                BoardAction(LoadedBoard(boardId, boxData.boxes, false)),
              );
            } else {
              dispatch(BoardAction(LoadedBoard(boardId, [], false)));
            };
          };
          Js.Promise.resolve();
        };
      None;
    },
    (dispatch, isAuthenticated),
  );
  {error: state.error, loading: state.loading};
};

let useMutation = (boardId: string): mutationResult => {
  let dispatch = AppStore.useDispatch();
  let (state, setState) =
    React.useState(_ => ({error: None, loading: false}: requestState));

  let m = m => {
    setState(prev => {...prev, loading: true});
    let result =
      switch (m) {
      | Add(box) =>
        switch (box.kind) {
        | Markdown(_) => Request.addMarkdown(boardId, box)
        | Web(_, _) => Request.addWebPage(boardId, box)
        | Image(_) => Request.addImage(boardId, box)
        | Pen(_, _, _, _) => Request.addPen(boardId, box)
        | Square(_) => Request.addSquare(boardId, box)
        | Arrow(_) => Request.addArrow(boardId, box)
        | _ => Request.addSquare(boardId, box)
        }
      | Update(box) =>
        switch (box.kind) {
        | Markdown(_) => Request.updateMarkdown(boardId, box)
        | Web(_, _) => Request.updateWebPage(boardId, box)
        | Image(_) => Request.updateImage(boardId, box)
        | Pen(_, _, _, _) => Request.updatePen(boardId, box)
        | Square(_) => Request.updateSquare(boardId, box)
        | Arrow(_) => Request.updateArrow(boardId, box)
        | _ => Request.updateSquare(boardId, box)
        }
      | Delete(box) =>
        switch (box.kind) {
        | Markdown(_) => Request.deleteMarkdown(box.id, box)
        | Web(_, _) => Request.deleteWebPage(box.id, box)
        | Image(_) => Request.deleteImage(box.id, box)
        | Pen(_, _, _, _) => Request.deletePen(box.id, box)
        | Square(_) => Request.deleteSquare(box.id, box)
        | Arrow(_) => Request.deleteArrow(box.id, box)
        | _ => Request.deleteSquare(box.id, box)
        }
      };

    let boxId =
      switch (m) {
      | Add(box) => box.id
      | Update(box) => box.id
      | Delete(box) => box.id
      };

    result
    ->Utils.Promise.then_(result => {
        switch (result) {
        | Ok(newId) when boxId != newId =>
          dispatch(BoardAction(UpdateBoxId(boxId, newId)));
          setState(_ => {error: None, loading: false});
        | Error(err) =>
          setState(_ =>
            {error: Some(Err.fromApolloError(err)), loading: false}
          )
        | _ => setState(_ => {error: None, loading: false})
        };
        Js.Promise.resolve();
      })
    ->Utils.Promise.ignore;
  };

  {error: state.error, loading: state.loading, mutation: m};
};
