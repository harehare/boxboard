open Box;

// [@bs.module] external marked: string => string = "marked";
[%bs.raw {|require('./Markdown.scss')|}];

let marked = [%bs.raw
  {|
  ((text) => {
    const marked = require('marked');
    const style = require('highlight.js/styles/github.css');
    marked.setOptions({
      renderer: new marked.Renderer(),
      highlight: function(code, language) {
        const hljs = require('highlight.js');
        const validLanguage = hljs.getLanguage(language) ? language : 'plaintext';
        return hljs.highlight(validLanguage, code).value;
      },
      pedantic: false,
      gfm: true,
      breaks: false,
      sanitize: false,
      smartLists: true,
      smartypants: false,
      xhtml: false
    });
    return marked(text);
  })
|}
];

[@react.component]
let make =
    (
      ~text: string,
      ~size: size,
      ~fontSize: int,
      ~isEdit: bool,
      ~onChange: ReactEvent.Form.t => unit,
    ) => {
  let (width, height) = size;

  if (isEdit) {
    <foreignObject x="10" y="10" width={j|$width|j} height={j|$height|j}>
      <textarea
        className="markdown"
        value=text
        placeholder="EDIT MARKDOWN"
        style={ReactDOMRe.Style.make(~fontSize={j|$(fontSize)px|j}, ())}
        onChange
      />
    </foreignObject>;
  } else {
    <foreignObject x="6" y="6" width={j|$width|j} height={j|$height|j}>
      <div
        className="markdown"
        style={ReactDOMRe.Style.make(~fontSize={j|$(fontSize)px|j}, ())}
        dangerouslySetInnerHTML={"__html": marked(text)}
      />
    </foreignObject>;
  };
};
