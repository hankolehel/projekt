import { Injectable } from '@angular/core';
import { Http } from '@angular/http';
import 'rxjs/add/operator/map';
import { URLSearchParams } from '@angular/http';

@Injectable()
export class PostsService {

  constructor(private http: Http) { }


  // Get all posts from the API
  getAllPosts() {
    return this.http.get('/api/posts')
      .map(res => res.json());
  }

  getCurrentData(carWorkingToggle?:boolean) {
      if (carWorkingToggle != null){
        let params: URLSearchParams = new URLSearchParams();
        params.set('carState', carWorkingToggle.toString());
        return this.http.get('/api/collect',{search: params})
                .map(res => res.json());
      }
      return this.http.get('/api/collect')
        .map(res => res.json());
    }

}
