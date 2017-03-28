import { Injectable } from '@angular/core';
import { Http } from '@angular/http';

@Injectable()
export class DataCollectionService {

  constructor(private http: Http) { }

  postData(){
  	return [1,2,3,4,5];
  }

}
