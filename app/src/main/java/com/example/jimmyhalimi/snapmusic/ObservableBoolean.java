package com.example.jimmyhalimi.snapmusic;

public class ObservableBoolean
{

  public interface OnBooleanChangeListener
  {
    public void onBooleanChanged(boolean newValue);
  }

  private OnBooleanChangeListener listener;

  public void setOnBooleanChangeListener(OnBooleanChangeListener listener)
  {
    this.listener = listener;
  }

  private boolean is_processing_;

  public void triggerBooleanListener(boolean is_processing_)
  {
    this.is_processing_ = is_processing_;

    if(listener != null)
    {
      listener.onBooleanChanged(is_processing_);
    }
  }
}