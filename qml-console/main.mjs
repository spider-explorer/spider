import { code2ary } from "./code2ary.mjs";
import { miniMAL } from "./miniMAL.mjs";

console.log("this is main.js!");
//console.log(sum(11, 22));
export function main(glob)
{
    console.log("func() called!");
    var ad = glob2.newApplicationData();
    console.log(ad.getTextFromCpp());
    var f = function(){};
    glob.log(typeof f);
    glob.log(typeof function(){});
    glob.log(f);
    console.log(ad.divide(5.0, 2.0));
    console.log(ad.divide(5.0, 0.0));
}
