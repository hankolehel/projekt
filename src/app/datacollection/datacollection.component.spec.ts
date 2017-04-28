/* tslint:disable:no-unused-variable */
import { async, ComponentFixture, TestBed } from '@angular/core/testing';
import { By } from '@angular/platform-browser';
import { DebugElement } from '@angular/core';

import { DatacollectionComponent } from './datacollection.component';

describe('DatacollectionComponent', () => {
  let component: DatacollectionComponent;
  let fixture: ComponentFixture<DatacollectionComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ DatacollectionComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(DatacollectionComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
