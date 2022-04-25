// @ts-ignore
import { int, double } from './xchange.types.ts';

export declare class Rectangle {
  width: int;
  height: int;
  constructor(width: int, height: int);
  area(): int;
}

export declare var rect1: Rectangle;
export declare let rect2: Rectangle;
export declare const rect3: Rectangle;
