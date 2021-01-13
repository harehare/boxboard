open Box;

[%bs.raw {|require('./Markdown.scss')|}];
[@bs.module] external sanitizeHtml: string => string = "sanitize-html";

let marked = [%bs.raw
  {|
  ((text) => {
    const marked = require('marked');
    const style = require('highlight.js/styles/github.css');
    const hljs = require('highlight.js/lib/core');
    const lang = [
      "apache",
      "applescript",
      "arduino",
      "autohotkey",
      "awk",
      "bash",
      "basic",
      "bnf",
      "brainfuck",
      "c",
      "ceylon",
      "clojure",
      "cmake",
      "coffeescript",
      "cpp",
      "crystal",
      "csharp",
      "css",
      "d",
      "dart",
      "delphi",
      "diff",
      "django",
      "dns",
      "dockerfile",
      "elixir",
      "elm",
      "erlang",
      "excel",
      "fsharp",
      "go",
      "gradle",
      "groovy",
      "haml",
      "haskell",
      "haxe",
      "hsp",
      "http",
      "ini",
      "java",
      "javascript",
      "json",
      "julia",
      "kotlin",
      "less",
      "lisp",
      "llvm",
      "lua",
      "makefile",
      "markdown",
      "matlab",
      "nginx",
      "nim",
      "nix",
      "node-repl",
      "objectivec",
      "ocaml",
      "perl",
      "pgsql",
      "php",
      "pony",
      "powershell",
      "processing",
      "prolog",
      "protobuf",
      "puppet",
      "python",
      "q",
      "r",
      "reasonml",
      "ruby",
      "rust",
      "scala",
      "scheme",
      "scss",
      "shell",
      "smalltalk",
      "sql",
      "swift",
      "thrift",
      "typescript",
      "vbnet",
      "vbscript",
      "vim",
      "xml",
      "yaml",
    ];
    for (const l of lang) {
      hljs.registerLanguage(l, require(`highlight.js/lib/languages/${l}`));
    }

    marked.setOptions({
      renderer: new marked.Renderer(),
      highlight: function(code, language) {
        const validLanguage = hljs.getLanguage(language) ? language : 'plaintext';
        return hljs.highlight(validLanguage, code).value;
      },
      pedantic: false,
      gfm: true,
      tables: true,
      breaks: false,
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
        dangerouslySetInnerHTML={"__html": sanitizeHtml(marked(text))}
      />
    </foreignObject>;
  };
};
