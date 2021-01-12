type icon = {
  prefix: string,
  iconName: string,
};

[@bs.module "@fortawesome/react-fontawesome"] [@react.component]
external make:
  (~icon: icon, ~className: string=?, ~color: string=?) => React.element =
  "FontAwesomeIcon";

[@bs.module "@fortawesome/fontawesome-svg-core"] [@bs.scope "library"]
external libraryAdd: icon => unit = "library";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faMousePointer: icon = "faMousePointer";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faPen: icon = "faPen";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faSquare: icon = "faSquare";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faStickyNote: icon = "faStickyNote";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faImage: icon = "faImage";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faGlobe: icon = "faGlobe";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faArrowRight: icon = "faArrowRight";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faCopy: icon = "faCopy";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faThumbtack: icon = "faThumbtack";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faTrash: icon = "faTrash";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faUpload: icon = "faUpload";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faPlusCircle: icon = "faPlusCircle";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faDownload: icon = "faDownload";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faUndo: icon = "faUndo";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faRedo: icon = "faRedo";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faMinus: icon = "faMinus";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faPlus: icon = "faPlus";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faSearch: icon = "faSearch";

[@bs.module "@fortawesome/free-solid-svg-icons"]
external faMap: icon = "faMap";

libraryAdd(faMousePointer);
libraryAdd(faPen);
libraryAdd(faSquare);
libraryAdd(faStickyNote);
libraryAdd(faImage);
libraryAdd(faGlobe);
libraryAdd(faArrowRight);
libraryAdd(faCopy);
libraryAdd(faTrash);
libraryAdd(faUpload);
libraryAdd(faPlusCircle);
libraryAdd(faDownload);
libraryAdd(faUndo);
libraryAdd(faRedo);
libraryAdd(faMinus);
libraryAdd(faPlus);
libraryAdd(faSearch);
libraryAdd(faMap);
