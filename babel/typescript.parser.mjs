import $parser from "https://jspm.dev/@babel/parser";

export class TypescriptParser {
  static async parseAsync(code) {
    var ast = $parser.parse(code, {
      sourceType: "module",
      plugins: ["typescript"],
    });
    return ast;
  }
  static async parseFileAsync(path) {
    var code = Deno.readTextFileSync(path);
    var ast = await this.parseAsync(code);
    return ast;
  }
}
