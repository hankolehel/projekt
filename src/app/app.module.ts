// Imports commented out for brevity
import { RouterModule }   from '@angular/router';
import { NgModule }       from '@angular/core';
import { BrowserModule }  from '@angular/platform-browser';
import { HttpModule }     from '@angular/http';
import { AppComponent }   from './app.component';
import { PostsComponent } from './posts/posts.component';
import { FormsModule }    from '@angular/forms';
import { PostsService } from './posts.service';

import { DataCollectionComponent } from './datacollection/datacollection.component';
import { DataCollectionService } from './datacollection.service';
import { ChartsModule } from 'ng2-charts';

// Define the routes
const ROUTES = [
  {
    path: '',
    redirectTo: 'posts',
    pathMatch: 'full'
  },
  {
    path: 'posts',
    component: PostsComponent
  },
  {
    path: 'data',
    component: DataCollectionComponent
  }
];

@NgModule({
  declarations: [
    AppComponent,
    PostsComponent,
    DataCollectionComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    HttpModule,
    ChartsModule,
    RouterModule.forRoot(ROUTES)
  ],
  providers: [PostsService, DataCollectionService],
  bootstrap: [AppComponent]
})
export class AppModule { }
