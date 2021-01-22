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
  | SaveBoard(Request.boardData)
  | DeleteBoard(Request.boardData)
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
        | Arrow(_) => Request.addArrow(boardId, box)
        | _ => Request.addMarkdown(boardId, box)
        }
      | Update(box) =>
        switch (box.kind) {
        | Markdown(_) => Request.updateMarkdown(boardId, box)
        | Web(_, _) => Request.updateWebPage(boardId, box)
        | Image(_) => Request.updateImage(boardId, box)
        | Pen(_, _, _, _) => Request.updatePen(boardId, box)
        | Arrow(_) => Request.updateArrow(boardId, box)
        | _ => Request.updateMarkdown(boardId, box)
        }
      | Delete(box) =>
        switch (box.kind) {
        | Markdown(_) => Request.deleteMarkdown(box.id, box)
        | Web(_, _) => Request.deleteWebPage(box.id, box)
        | Image(_) => Request.deleteImage(box.id, box)
        | Pen(_, _, _, _) => Request.deletePen(box.id, box)
        | Arrow(_) => Request.deleteArrow(box.id, box)
        | _ => Request.deleteMarkdown(box.id, box)
        }
      | SaveBoard(boardData) => Request.saveBoard(boardData)
      | DeleteBoard(boardData) => Request.deleteBoard(boardData)
      };

    let boxId =
      switch (m) {
      | Add(box) => box.id
      | Update(box) => box.id
      | Delete(box) => box.id
      | _ => ""
      };

    result
    ->Utils.Promise.then_(result => {
        switch (result) {
        | Ok(newId) =>
          switch (m) {
          | Add(_) =>
            dispatch(BoardAction(UpdateBoxId(boxId, newId)));
            setState(_ => {error: None, loading: false});
          | Update(_) =>
            dispatch(BoardAction(UpdateBoxId(boxId, newId)));
            setState(_ => {error: None, loading: false});
          | Delete(_) =>
            dispatch(BoardAction(UpdateBoxId(boxId, newId)));
            setState(_ => {error: None, loading: false});
          | _ => setState(_ => {error: None, loading: false})
          }
        | Error(err) =>
          setState(_ =>
            {error: Some(Err.fromApolloError(err)), loading: false}
          )
        };
        Js.Promise.resolve();
      })
    ->Utils.Promise.ignore;
  };

  {error: state.error, loading: state.loading, mutation: m};
};

let useQuery = (boardId: string): queryResult => {
  let dispatch = AppStore.useDispatch();
  let (state, setState) =
    React.useState(_ => ({error: None, loading: false}: requestState));

  let (localBoardValue, _) = useLocalStorage(boardId);
  let {isAuthenticated, getIdTokenClaims} = Auth0.useAuth0();
  let {error, loading, mutation} = useMutation(boardId);

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
                  | Ok(res)
                      when
                        switch (res.remoteId) {
                        | Some(_) => true
                        | None => false
                        } =>
                    dispatch(BoardAction(LoadedBoard(res)));
                    setState(_ => {error: None, loading: false});
                    Js.Promise.resolve();
                  | Ok(res) =>
                    dispatch(BoardAction(LoadedBoard(res)));
                    mutation(
                      SaveBoard({
                        remoteId: None,
                        boardId,
                        title: None,
                        position: (0, 0),
                        scale: 1.0,
                      }),
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
          | None =>
            dispatch(
              BoardAction(
                LoadedBoard({
                  remoteId: None,
                  boardId,
                  title: None,
                  boxes: [],
                  scale: 1.0,
                  position: (0, 0),
                }),
              ),
            )
          | Some(j) =>
            let result = Js.Json.parseExn(j) |> Box.boxData_decode;

            switch (result) {
            | Ok(v) =>
              dispatch(
                BoardAction(
                  LoadedBoard({
                    remoteId: None,
                    boardId,
                    title: v.title,
                    boxes: v.boxes,
                    scale: v.scale,
                    position: v.position,
                  }),
                ),
              )
            | Error(_) =>
              dispatch(
                BoardAction(
                  LoadedBoard({
                    remoteId: None,
                    boardId,
                    title: None,
                    boxes: [],
                    scale: 1.0,
                    position: (0, 0),
                  }),
                ),
              )
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
